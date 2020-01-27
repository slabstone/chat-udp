add_executable(server)

target_sources(server PRIVATE
  main.cpp
  mainwindow.h mainwindow.cpp mainwindow.ui
  server.h server.cpp)

target_link_libraries(server PRIVATE Qt4::QtCore Qt4::QtGui Qt4::QtNetwork)
