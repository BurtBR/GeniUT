# GeniUT
Um jogo semelhante ao Genius mas com um alcance maior de notas. São 60 notas divididas em 5 oitavas de escalas cromáticas.

## Instalando
* Vá para a página de Releases: https://github.com/BurtBR/GeniUT/releases
* Baixe o .zip da última versão
* Extraia os arquivos em alguma pasta
* Execute o arquivo GeniUT.exe

## Observação
* O windows defender pode deletar temporariamente o executável após extração para verificar a existência de vírus. Aguarde esse processo.
* Ao executar, o windows defender pode bloquear a execução por não conhecer a origem do aplicativo. Nesse caso clique em saber mais e depois em executar mesmo assim.

## Modos
* **Praticar**: Nesse modo você pode testar as músicas salvas sem ter que reiniciar a partida quando falhar. O placar não é registrado nesse modo.
* **Música**: Nesse modo de jogo você deve reproduzir corretamente a ordem das notas da música escolhida pelo programa. Caso a música chegue ao fim, inicia-se o modo aleatório.
* **Aleatório**: Nesse modo de jogo você deve reproduzir corretamente a ordem das notas escolhidas aleatoriamente pelo programa.
* **Criar**: Nesse modo de jogo o primeiro jogador escolhe a primeira nota da partida. Nas jogadas seguintes, cada jogador deve repetir a sequência anterior e adicionar uma nova nota ao final.
* **Compor**: Esse modo é utilizado para compor as músicas que serão utilizadas pelo programa. Além de compor, é possível editar o tempo e as notas de músicas já existentes.

## Atalhos do teclado
* Tons: Q,W,E,R,A,S,D,F,Z,X,C,V
* Voltar: M
* Oitava acima: =
* Oitava abaixo: -
* Selecionar oitava diretamente: 2,3,4,5,6

## Linux - PiOS x64 (versão compilada)
* A versão compilada, disponível nos Releases desse repositório, parte do princípio de que os componentes do QT necessários estão instalados e que a estrutura de arquivos é específica.
* Para que funcione corretamente, o usuário do dispositivo precisa ser "geniut", possibilitando que o aplicativo encontre as dependências do QT no mesmo endereço do dispositivo em que foi compilado.
* Certifique-se de que possui os componentes necessários para instalar o QT:
  > sudo apt-get install build-essential libgl1-mesa-dev
  
  > sudo apt-get install libxcb-cursor-dev
  
  > sudo apt-get install cmake
* Instale o gstreamer com o pulseaudio:
  > sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
* Faça o download da ferramenta de instalação do QT, **selecione o componente "Qt Desktop" e a biblioteca adicional "Qt Multimedia"**
  > https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-online-installer-linux-arm64-4.8.1.run
* Após a instalação do QT, extraia o conteúdo do arquivo GeniUT_1_0_2.zip em uma pasta na área de trabalho de nome "GeniUT". O arquivo executável deve estar no seguinte caminho:
  > /home/geniut/Desktop/GeniUT/GeniUT.run

## Linux - PiOS x64 (compilar no dispositivo)
* Certifique-se de que possui os componentes necessários para instalar o QT:
  > sudo apt-get install build-essential libgl1-mesa-dev
  
  > sudo apt-get install libxcb-cursor-dev
  
  > sudo apt-get install cmake
* Instale o gstreamer com o pulseaudio:
  > sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
* Faça o download da ferramenta de instalação do QT, **selecione o componente "Qt Desktop" e a biblioteca adicional "Qt Multimedia"**
  > https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-online-installer-linux-arm64-4.8.1.run
* Clone este repositório em alguma pasta
  > git clone https://github.com/BurtBR/GeniUT.git .
* Abra um terminal e navegue até a pasta bin da sua instalação do QT, por exemplo:
  > cd /home/nomedoseuusuario/Qt/6.7.3/gcc_amr64/bin
* Rode a configuração do cmake na pasta do repositório do GeniUT clonada
  > ./qt-cmake -G Ninja -S \<Pasta do repositório\> -B \<Pasta de destino do programa compilado\>
  * Exemplo:
    > ./qt-cmake -G Ninja -S /home/usuario/Desktop/GeniUT -B /home/usuario/Desktop/GeniUT/Build
* Navegue para a pasta de compilação
  > cd \<Pasta de destino do programa compilado\>
* Compile o programa através do cmake ou ninja, limitando a quantidade de jobs possíveis para que o compilador não exceda a memória do dispositivo (--parallel \<quantidade\> ou -j \<quantidade\>)
  > ninja -j 1

  ou
  
  > cmake --build . --parallel 1
* Execute o programa GeniUT gerado nessa pasta
