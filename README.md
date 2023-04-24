# losshd
Zabbix external check for very low packet loss monitoring

Build:


cd build
conan install .. -of ../conan
cmake .. -DCMAKE_TOOLCHAIN_FILE=../conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .


Don't forget to create table from file schema.sql on your PostgreSQL server!


Dependencies:

Boost.Program_Options
libpqxx
libpq


