#include <SD.h>
#include <SPI.h>

File Arquivo;

int pinoSS = 10; // Pin 53 para Mega / Pin 10 para UNO

void setup() {
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(pinoSS, OUTPUT);

  // Inicialização simulada do "SD Card"
  Serial.println("Simulação: Sistema de Arquivo pronto para uso.");
  SimulaArquivos();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equals("ls")) {
      Serial.println("Simulação: Conteúdo do diretório:");
      Lendo_Diretorio();
    } else if (input.startsWith("touch")) {
      String fileName = input.substring(6); // Ignora o comando "touch "
      CriarArquivo(fileName);
    } else if (input.startsWith("mkdir")) {
      String dirName = input.substring(6); // Ignora o comando "mkdir "
      CriarDiretorio(dirName);
    } else if (input.startsWith("cat")) {
      String fileName = input.substring(4); // Ignora o comando "cat "
      LerArquivo(fileName);
    } else if (input.startsWith("edit")) {
      // Comando edit arquivo.txt "Este é um texto"
      String comando = input.substring(5); // Ignora o comando "edit "
      int posicaoAspas1 = comando.indexOf("\"");
      int posicaoAspas2 = comando.indexOf("\"", posicaoAspas1 + 1);

      if (posicaoAspas1 != -1 && posicaoAspas2 != -1) {
        String fileName = comando.substring(0, posicaoAspas1);
        String novoConteudo = comando.substring(posicaoAspas1 + 1, posicaoAspas2);

        EditarArquivo(fileName, novoConteudo);
      } else {
        Serial.println("Simulação: Comando 'edit' inválido. Use 'edit arquivo.txt \"Este é um texto\"'");
      }
    } else if (input.equals("info")) {
      ExibirInformacoes();
    }
  }
}

// Função de simulação: cria alguns arquivos fictícios
void SimulaArquivos() {
  SD.begin(pinoSS); // Inicializa (simulado)
  Arquivo = SD.open("arquivo1.txt", FILE_WRITE);
  Arquivo.println("Conteúdo do arquivo 1");
  Arquivo.close();
  Arquivo = SD.open("arquivo2.txt", FILE_WRITE);
  Arquivo.println("Conteúdo do arquivo 2");
  Arquivo.close();
}

// Função de leitura de todos os arquivos fictícios
void Lendo_Diretorio() {
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    Serial.println(entry.name());
    entry.close();
  }
  Serial.println(" ");
}

// Função para criar um arquivo
void CriarArquivo(String fileName) {
  Arquivo = SD.open(fileName, FILE_WRITE);
  Arquivo.println("Novo arquivo criado!");
  Arquivo.close();
  Serial.println("Simulação: Arquivo criado: " + fileName);
}

// Função para criar um diretório
void CriarDiretorio(String dirName) {
  if (SD.mkdir(dirName)) {
    Serial.println("Simulação: Diretório criado: " + dirName);
  } else {
    Serial.println("Simulação: Falha ao criar diretório: " + dirName);
  }
}

// Função para ler e exibir o conteúdo de um arquivo
void LerArquivo(String fileName) {
  Arquivo = SD.open(fileName);
  if (Arquivo) {
    while (Arquivo.available()) {
      Serial.write(Arquivo.read());
    }
    Arquivo.close();
  } else {
    Serial.println("Simulação: Falha ao abrir o arquivo: " + fileName);
  }
}

// Função para editar um arquivo
void EditarArquivo(String fileName, String novoConteudo) {
  Arquivo = SD.open(fileName, FILE_WRITE);
  if (Arquivo) {
    Arquivo.println(novoConteudo);
    Arquivo.close();
    Serial.println("Simulação: Arquivo editado: " + fileName);
  } else {
    Serial.println("Simulação: Falha ao abrir o arquivo para edição: " + fileName);
  }
}

// Função para obter a quantidade de memória livre
int freeMemory() {
  char stack_dummy = 0;
  return &stack_dummy - reinterpret_cast<char*>(malloc(1));
}

void ExibirInformacoes() {
  Serial.println("KittyOS v0.1");
  Serial.println("Arduino UNO");

  // Obtendo informações sobre o clock
  unsigned long clockSpeed = F_CPU;
  float clockMHz = clockSpeed / 1000000.0;
  Serial.print("Clock Speed: ");
  Serial.print(clockSpeed);
  Serial.print(" Hz (");
  Serial.print(clockMHz, 2);
  Serial.println(" MHz)");

  // Obtendo informações sobre a memória livre
  int freeMemoryValue = freeMemory();
  Serial.print("Memória livre: ");
  Serial.print(freeMemoryValue);
  Serial.println(" bytes");
}
