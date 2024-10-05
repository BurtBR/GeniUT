# GeniUT
Um jogo semelhante ao Genius mas com um alcance maior de notas. São 60 notas divididas em 5 oitavas de escalas cromáticas.

## Instalação - Windows
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
* Faça o download da ferramenta de instalação do QT, **selecione o componente "Qt Desktop" e a biblioteca adicional "Qt Multimedia"**. Lembrando que é possível realizar o download pelo terminal através do WGET.
  > https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-online-installer-linux-arm64-4.8.1.run
* Após a instalação do QT, extraia o conteúdo do arquivo GeniUT_1_0_3.zip em uma pasta na área de trabalho de nome "GeniUT". O arquivo executável deve estar no seguinte caminho:
  > /home/geniut/Desktop/GeniUT/GeniUT.run

## Linux - PiOS x64 (compilar no dispositivo)
* Certifique-se de que possui os componentes necessários para instalar o QT:
  > sudo apt-get install build-essential libgl1-mesa-dev
  
  > sudo apt-get install libxcb-cursor-dev
  
  > sudo apt-get install cmake
* Instale o gstreamer com o pulseaudio:
  > sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
* Faça o download da ferramenta de instalação do QT, **selecione o componente "Qt Desktop" e a biblioteca adicional "Qt Multimedia"**. Lembrando que é possível realizar o download pelo terminal através do WGET.
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

## Problemas Comuns - PiOS
* "Não há músicas para praticar"
  * Verifique se existe uma pasta "Musicas" em /home/nomedousuario e ela está vazia. Nesse caso, o aplicativo está rodando em /home/nomedousuario, mova as músicas da pasta GeniUT/Musicas para /home/nomedousuario/Musicas
* Aplicativo sem som
  * Verifique se a saída de som do dispositivo está correta (em "AV Jack" por exemplo)
  * Verifique se o gstreamer e o pulseaudio estão instalados
* Desempenho ruim
  * A resolução da tela pode ter um grande impacto no desempenho do dispositivo. Tente reduzir.
  * Desligue o Wifi e Bluetooth

## Iniciar o programa automaticamente - PiOS
* Crie uma pasta autostart em /home/usuario/.config
  > mkdir /home/usuario/.config/autostart
* Crie um arquivo GeniUT.desktop nessa pasta
  > cd /home/usuario/.config/autostart <br>
  > touch GeniUT.desktop
* Abra o arquivo, coloque os dados de acordo com o local do aplicativo e salve
  > nano GeniUT.desktop

  > [Desktop Entry] <br>
  > Type=Application <br>
  > Name=GeniUT <br>
  > Path=/home/geniut/Desktop/GeniUT <br>
  > Exec=/home/geniut/Desktop/GeniUT/GeniUT <br>
* Reinicie o dispositivo
  > sudo reboot

## Pinos Raspberry Pi

  |3.3V |o  o| 5V |
  |--:|:--:|:--|
  |__GPIO2__ |__o  o__| __5V__ |
  |__GPIO3__ |__o  o__| __GND__ |
  |__GPIO4__ |__o  o__| __GPIO14__ |
  |__GND__ |__o  o__| __GPIO15__ |
  |__GPIO17__ |__o  o__| __GPIO18__ |
  |__GPIO27__ |__o  o__| __GND__ |
  |__GPIO22__ |__o  o__| __GPIO23__ |
  |__3.3V__ |__o  o__| __GPIO24__|
  |__GPIO10__ |__o  o__| __GND__ |
  |__GPIO9__ |__o  o__| __GPIO25__ |
  |__GPIO11__ |__o  o__| __GPIO8__ |
  |__GND__ |__o  o__| __GPIO7__ |
|__RESERVED__ |__o  o__| __RESERVED__ |
  |__GPIO5__ |__o  o__| __GND__ |
  |__GPIO6__ |__o  o__| __GPIO12__ |
  |__GPIO13__ |__o  o__| __GND__ |
  |__GPIO19__ |__o  o__| __GPIO16__ |
  |__GPIO26__ |__o  o__| __GPIO20__ |
  |__GND__ |__o  o__| __GPIO21__ |

Teclado de LEDs:

| GPIO17 | GPIO27 | GPIO22 | GPIO5  |
|---|---|---|---|
| __GPIO6__  | __GPIO13__ | __GPIO26__ | __GPIO12__ |
| __GPIO25__ | __GPIO24__ | __GPIO23__ | __GPIO18__ |

Circuito Proposto:

![alt text](https://github.com/BurtBR/GeniUT/blob/master/CircuitDiagram.png?raw=true)
