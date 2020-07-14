cmake_minimum_required(VERSION 3.0)
project(beh_app)
set(SOURCE main.cpp factory.cpp node.cpp drawer.cpp flyweight.cpp app.cpp action.cpp state.cpp config.cpp visitor.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})