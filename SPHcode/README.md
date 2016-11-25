# SPHcode

Implementação em C++ do método SPH realizado pelos alunos de iniciação científica do grupo USPhydro.

## Resumo

A estrutura do código está dividida nas pastas `/src` e `/tests`. Iremos discutir sobre essa estrutura no dia (30/11/2016).

Frameworks utilizados:

- [CMake](https://cmake.org/) - An open-source, cross-platform family of tools designed to build, test and package software.
- [GTest](https://github.com/google/googletest) - A unit testing library for the C++ programming language, based on the xUnit architecture.
- [Doxygen](http://www.stack.nl/~dimitri/doxygen/) - Is the standard tool for generating documentation from annotated C++ sources.


## Instalação

### gtest

O [gtest](https://github.com/google/googletest) é um framework desenvolvido pelo Google para construção de testes de unidade (unit tests) em C++.

#### Ubuntu

Instale o pacote de desenvolvimento:

```bash
sudo apt-get install libgtest-dev
```

Esse pacote somente instala os arquivos fonte. Você ainda vai precisar compilar o pacote utilizando o cmake.

```bash
sudo apt-get install cmake # install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make

# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib
```

Com isso o gtest deve estar instalado. Para testá-lo, rode os testes de unidade do `SPHcode`. Vá até a pasta `SPHcode` e rode os seguintes comandos:

```bash
cmake CMakeLists.txt
make
./runTests
```

#### MacOs

Não tive sucesso em instalar no MacOs Sierra. Nesta última atualização foram alterados alguns caminhos (`/usr/local`, por exemplo) e os desenvolvedores do gtest ainda não ajustaram essas alterações.

### Windows

Se alguém conseguir, coloque o passo-a-passo aqui.

## Desenvolvimento

Iremos utilizar o [Test Driven Development](http://tdd.caelum.com.br/).

## Colaboradores
