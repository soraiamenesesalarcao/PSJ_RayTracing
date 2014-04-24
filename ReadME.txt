--------------------------------------------------------------------------------
-----------[ Programação 3D para Simulação de Jogos - Assignment 3 ]------------
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
--------- Grupo 4 --------------------------------------------------------------
--------------------------------------------------------------------------------
- 57690 - Soraia Meneses Alarcão -----------------------------------------------
- 64052 - Cátia Dias -----------------------------------------------------------
- 68202 - Vânia Mendonça -------------------------------------------------------
--------------------------------------------------------------------------------


----------------[ Estrutura de directórios e bibliotecas ]----------------------

Os ficheiros submetidos dizem respeito ao código C++ da aplicação, os quais
 destinam-se a ser integrados num projecto de Visual Studio.

Para a concretização deste projecto, foram utilizadas as seguintes bibliotecas:
- glut
- glm

Uma sugestão de integração do código submetido com as bibliotecas necessárias 
consiste na seguinte estutura de pastas:

- dependencies
	--> Aqui entram as diversas bibliotecas

- bin
	--> Aqui entram os ficheiros executáveis da aplicação e o ficheiro
		glut32.dll

- scripts
	--> Aqui entra o ficheiro .sln do Visual Studio

- projects
	- <nome_do_projecto>
		- scripts
			--> aqui entram a pasta "resources" (onde se encontram os ficheiros 
				.nff) e ainda o ficheiro .vsproj
		- src [pasta submetida]
			--> aqui entra o código submetido na pasta src
		- obj
			--> Aqui entram os ficheiros de debug gerados pelo VS


------------------------[ Interacção com a aplicação ]--------------------------

Os comandos suportados pela aplicação são os seguintes:

- A - Activar / desactivar o anti-aliasing
- S - Activar / desactivar as soft shadows
- D - Activar / desactivar a depth of field
- G - Activar / desactivar a regular grid
- T - Activar / desactivar o uso de threads


--------------------------------------------------------------------------------

-------------------[ Carregamento de ficheiros externos]------------------------

A aplicação suporta a leitura de um ficheiro NFF que pode conter uma cor de fundo,
uma câmara, fontes de luz, materiais e primitivas (esferas, planos, polígonos 
e patches de polígonos).

Sugere-se que os ficheiros externos a carregar sejam colocados numa pasta 
"resources", incluída dentro da pasta onde se encontra o ficheiro .vsproj 
(uma vez que é aí que se vai buscar o ficheiro .nff). A alteração explícita do 
ficheiro a carregar é feita na classe Scene.