cmake_minimum_required(VERSION 3.16)

#Название проекта
project(SectorIntersectionApp VERSION 0.1 LANGUAGES CXX)

#Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Гарантия работы стандарта C++

#Поддержка Qt6
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.7.2_1")
find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

#Автоматическая обработка MOC, UI и RCC файлов (Qt)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#Настройки компиляции
add_compile_options(-Wall -Wextra -Wpedantic)
if (MSVC)
    add_compile_options(/W4)
endif()

#Google Test
find_package(GTest REQUIRED)

#Основные исходные файлы
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    geometry.h
    sector.h
    intersectioncalculator.h
    visualizationwidget.h
)

#Создание основного исполняемого файла
qt_add_executable(SectorIntersectionApp
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

#Линковка с Qt
target_link_libraries(SectorIntersectionApp PRIVATE Qt6::Widgets)

#Настройки для macOS
if (APPLE)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SectorIntersectionApp)
    set_target_properties(SectorIntersectionApp PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )
endif()

#Настройки для Windows
if (WIN32)
    set_target_properties(SectorIntersectionApp PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

#Добавление тестов
add_executable(tst_mainfunctionality tst_mainfunctionality.cpp)

#Линковка тестов с GTest и Qt6
target_link_libraries(tst_mainfunctionality PRIVATE GTest::GTest GTest::Main Qt6::Widgets Qt6::Test)

#Установка исполняемых файлов
install(TARGETS SectorIntersectionApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#Финализация Qt
qt_finalize_executable(SectorIntersectionApp)
