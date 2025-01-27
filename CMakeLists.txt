cmake_minimum_required(VERSION 3.10)

# Название проекта и язык программирования
project(MySDL2Project C CXX)

# Включаем поддержку pkg-config для поиска библиотек
find_package(PkgConfig REQUIRED)

# Поиск библиотек SDL2, SDL2_image и SDL2_ttf через pkg-config
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Добавляем исполняемый файл проекта
add_executable(my_sdl_project src/main.cpp)

# Добавляем пути к заголовочным файлам
target_include_directories(my_sdl_project PRIVATE 
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS} 
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Линкуем библиотеки SDL2, SDL2_image, SDL2_ttf
target_link_libraries(my_sdl_project 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES}
)

# Вывод информации о найденных библиотеках
message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image libraries: ${SDL2_IMAGE_LIBRARIES}")
message(STATUS "SDL2_ttf libraries: ${SDL2_TTF_LIBRARIES}")
