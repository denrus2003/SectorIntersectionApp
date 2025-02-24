cmake_minimum_required(VERSION 3.16)

# Название проекта
project(SectorIntersectionApp VERSION 0.1 LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Гарантия работы стандарта C++

# Поддержка Qt6
include_directories(/opt/homebrew/Cellar/qt/6.7.2_1/include)
include_directories(/opt/homebrew/Cellar/qt/6.7.2_1/include/QtWidgets)
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.7.2_1")
find_package(Qt6 REQUIRED COMPONENTS Widgets Test Gui Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets Test Gui Core)
include_directories(/opt/homebrew/Cellar/qt/6.7.2_1/include/QtGui)
include_directories(/opt/homebrew/Cellar/qt/6.7.2_1/include/QtCore)

# Автоматическая обработка MOC, UI и RCC файлов (Qt)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Настройки компиляции
add_compile_options(-Wall -Wextra -Wpedantic)
if (MSVC)
    add_compile_options(/W4)
endif()

# Google Test
find_package(GTest REQUIRED)

# Основные исходные файлы
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    geometry.h
    sector.h
    intersectioncalculator.h
    visualizationwidget.h
)

# Тестовые файлы
set(TEST_SOURCES
    tst_mainfunctionality.cpp
)

# Создание библиотеки
add_library(SectorIntersectionApp STATIC ${PROJECT_SOURCES})

# Создание исполняемого файла
add_executable(SectorIntersectionAppExecutable main.cpp)
target_link_libraries(SectorIntersectionAppExecutable PRIVATE SectorIntersectionApp)
target_link_libraries(SectorIntersectionApp PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Test GTest::GTest GTest::Main)

# Финализация Qt
qt_finalize_executable(SectorIntersectionAppExecutable)

# Создание тестового исполняемого файла
add_executable(TestRunner ${TEST_SOURCES})
target_link_libraries(TestRunner PRIVATE Qt6::Test SectorIntersectionApp GTest::GTest GTest::Main)
add_test(NAME TestRunner COMMAND TestRunner)

# Линковка с Qt и GTest
target_link_libraries(SectorIntersectionApp PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core Qt6::Test GTest::GTest GTest::Main)

# Настройки для macOS
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SectorIntersectionApp)
    set_target_properties(SectorIntersectionAppExecutable PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )
endif()

# Настройки для Windows
if (WIN32)
    set_target_properties(SectorIntersectionAppExecutable PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# Установка исполняемых файлов
install(TARGETS SectorIntersectionAppExecutable
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
