## Графы

зависимости: cmake, python3, c++17

---
* build:
```
cmake -B build
cmake --build build
./build/graphs
```
* test
```
cmake -B build
cmake --build . --target check --
```

В папке с тестами лежит генератор, но он умеет только генерировать тесты, ответы надо делать руками:(