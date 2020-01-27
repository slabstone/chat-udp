add_executable(client)

target_sources(client PRIVATE
  main.cpp
  mainwindow.h mainwindow.cpp mainwindow.ui
  client.h client.cpp)

target_link_libraries(client PRIVATE Qt4::QtCore Qt4::QtGui Qt4::QtNetwork)
