# =============================================================================
#  Somnia Curatio — CMake build script
#  SFML 3.0.2 + tmxlite
# =============================================================================

# Требуем CMake 3.22+ (3.22 — первая версия, корректно работающая с SFML 3 через
# FetchContent; ниже ломается из-за target exports).
cmake_minimum_required(VERSION 3.22)

# --- Совместимость с CMake 4.0 ---
# CMake 4.0 (2025) убрал поддержку cmake_minimum_required < 3.5. У tmxlite
# внутри написано cmake_minimum_required(VERSION 2.8) — старьё, которое мы
# не можем править. Этот флаг говорит CMake: "если субпроект требует меньше
# чем 3.5, считай как 3.5". Даёт собираться даже на свежей CMake 4.x.
# Строку НУЖНО ставить ДО project() и ДО FetchContent.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

# Объявляем проект. LANGUAGES CXX — нам нужен только C++ (не C, не CUDA и т.д.).
project(SomniaCuratio LANGUAGES CXX)

# --- Стандарт языка --------------------------------------------------------
# ТЗ практики не запрещает C++20; он нам даёт std::format, designated inits,
# std::span и удобные концепты. Если преподаватель требует строго C++17 —
# замени 20 на 17 (ничего ниже не используем).
set(CMAKE_CXX_STANDARD 20)
# REQUIRED = если компилятор не поддерживает стандарт, cmake сразу упадёт.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# EXTENSIONS OFF = запрещаем GNU-расширения (gnu++20), только чистый стандарт.
# Нужно, чтобы код одинаково собирался на MSVC/Clang/GCC.
set(CMAKE_CXX_EXTENSIONS OFF)

# Вывод компилятора вида "build/SomniaCuratio.exe" рядом с ассетами при MSVC.
# Иначе VS кладёт .exe в Debug/ или Release/ — и relative пути к assets/ ломаются.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# --- Зависимости -----------------------------------------------------------
# FetchContent автоматически скачивает и собирает зависимости. Удобно, потому
# что преподавателю не нужно ничего доустанавливать, чтобы собрать твой проект.
include(FetchContent)

# SFML 3.0.2
# GIT_SHALLOW = качаем только одну ревизию (без истории), быстрее и легче.
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
    GIT_SHALLOW    TRUE
)

# tmxlite — парсер Tiled-карт. Работает с SFML 3.
# SFML_ENABLE_GRAPHICS у tmxlite выключен (мы сами рисуем через sf::VertexArray),
# это даст контроль над шейдерами/батчингом.
set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG        v1.4.4
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  tmxlite   # библиотека лежит во вложенной папке
)

# nlohmann/json — будем читать диалоги из .json. Header-only, компиляции нет.
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)

# Собираем все три зависимости. MakeAvailable подгрузит и добавит как subdirectory.
FetchContent_MakeAvailable(SFML tmxlite nlohmann_json)

# --- Исходники игры --------------------------------------------------------
# Список файлов перечисляем явно (не GLOB). CMake не отслеживает появление
# новых файлов при GLOB — легко забыть пересобрать. Явный список безопаснее.
set(SOMNIA_SOURCES
    src/main.cpp
    src/core/Game.cpp
    src/core/SceneManager.cpp
    src/scenes/BureauScene.cpp
    src/world/TileMap.cpp
    src/world/Camera.cpp
    src/entities/Player.cpp
    src/entities/NPC.cpp
    src/ui/DialogueManager.cpp
    src/scenes/ArachnophobiaScene.cpp
    src/entities/Spider.cpp
    src/entities/BossSpider.cpp
)

# Заголовки перечисляем отдельно — они не компилируются, но попадут в
# список файлов в Visual Studio (вкладка "Solution Explorer"), чтобы
# можно было открывать их двойным кликом.
set(SOMNIA_HEADERS
    src/core/Game.h
    src/core/SceneManager.h
    src/scenes/Scene.h
    src/scenes/BureauScene.h
    src/world/TileMap.h
    src/world/Camera.h
    src/entities/Player.h
)

# add_executable создаёт бинарник.
# ВАЖНО: флаг WIN32 здесь НЕ ставим. Если поставить — MSVC переключится на
# подсистему Windows и начнёт искать точку входа WinMain() вместо обычной
# int main(). У нас main.cpp с обычной int main() → получили бы ошибку
# LNK2019: unresolved external symbol WinMain. На этапе разработки рядом с
# игровым окном откроется чёрная консоль — это нормально, туда удобно
# печатать отладку (std::cout/std::cerr). Чтобы в релизной версии спрятать
# консоль, можно будет позже либо написать WinMain-обёртку, либо добавить
# WIN32 + линкер-флаг /ENTRY:mainCRTStartup.
add_executable(SomniaCuratio ${SOMNIA_SOURCES} ${SOMNIA_HEADERS})

# --- Пути и линковка -------------------------------------------------------
# include директория для наших заголовков
target_include_directories(SomniaCuratio PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Линкуем SFML 3 (модули Graphics/Window/System/Audio) + tmxlite + json.
# PRIVATE = зависимости не просачиваются наружу (у нас нет "наружу", но
# это хорошая привычка).
target_link_libraries(SomniaCuratio PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    tmxlite
    nlohmann_json::nlohmann_json
)

# --- Копирование ассетов ---------------------------------------------------
# После сборки копируем папки assets/ и data/ рядом с .exe.
# Иначе игра не найдёт карты, спрайты и диалоги в runtime.
add_custom_command(TARGET SomniaCuratio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:SomniaCuratio>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:SomniaCuratio>/data"
)

# --- Предупреждения --------------------------------------------------------
# Строгие предупреждения помогают ловить баги на этапе компиляции.
if(MSVC)
    # /W4 — максимальный уровень предупреждений MSVC (но не /Wall, он шумный)
    # /permissive- — строгое соответствие стандарту
    # /Zc:__cplusplus — MSVC без этого флага врёт про версию стандарта
    target_compile_options(SomniaCuratio PRIVATE /W4 /permissive- /Zc:__cplusplus)
else()
    target_compile_options(SomniaCuratio PRIVATE -Wall -Wextra -Wpedantic)
endif()
