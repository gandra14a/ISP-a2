#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLOCK_SIZE 100

char* getUserInput(int* return_value);

int getNumberOfSentences(char* user_input);

int getLength(char* string);

int lastSentenceSymbolMissing(char* user_input);

char** getSentences(char* user_input, int* return_value);

char getCommandFromUser();

void printTextWithoutNumber(char** sentences, int number_of_sentences);

void printTextWithNumber(char** sentences, int number_of_sentences);

void freeMemory(char* user_input, char** sentences, int number_of_sentences);

void printTextStatistic(char** sentences, int number_of_sentences);

void switchTwoSentences(char*** sentences, int number_of_sentences);

void concatenateTwoSentences(char*** sentences, int* number_of_sentences, int* return_value);





int main()
{

  int return_value = 0;

  char *whole_text = getUserInput(&return_value);

  if (whole_text == NULL)
  {
    return return_value;
  }

  int num_sentences = getNumberOfSentences(whole_text);

  if (num_sentences == 0)
  {
    printf("Kein Satz gefunden!\n");
    free(whole_text);
    return 1;
  }

  if (lastSentenceSymbolMissing(whole_text) == 1)
  {
    printf("Der Text endet ohne Satzzeichen!\n");
    free(whole_text);
    return 2;
  }

  char** all_sentences = getSentences(whole_text, &return_value);

  if (all_sentences == NULL)
  {
    return return_value;
  }
  
  char user_option = getCommandFromUser();


  while (1)
  {
    switch(user_option)
    {
      case 't':
        printTextStatistic(all_sentences, num_sentences);
        break;
      case 's':
        switchTwoSentences(&all_sentences, num_sentences);
        break;
      case 'c':
        concatenateTwoSentences(&all_sentences, &num_sentences, &return_value);
        if (return_value == 3)
        {
          freeMemory(whole_text, all_sentences, num_sentences);
          return 3;
        }
        break;
      case 'p':
        printTextWithNumber(all_sentences, num_sentences);
        break;
      case 'o':
        printTextWithoutNumber(all_sentences, num_sentences);
        break;
      case 'e':
        printf("Programmende!\n");
        freeMemory(whole_text, all_sentences, num_sentences);
        return 0;
    }
    user_option = getCommandFromUser();
  }

}








char* getUserInput(int* return_value)
{

  char *buffer = (char*) malloc((BLOCK_SIZE - 1) * sizeof(char));

  if (buffer == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_value = 3;
    return buffer;
  }

  char *buffer_block = (char*) malloc(BLOCK_SIZE * sizeof(char));

  if (buffer_block == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_value = 3;
    free(buffer);
    return buffer_block;
  }

  char *new_buffer = NULL;

  printf("Bitte Text eingeben:\n");

  int new_line_found = 0;
  int number_of_blocks = 0;
  int text_len = 0;

  while (!new_line_found)
  {
    fgets(buffer_block, BLOCK_SIZE, stdin);

    for (int i = 0; i < (BLOCK_SIZE - 1); i++)
    {
      if (buffer_block[i] == '\n')
      {
        new_line_found = 1;
        break;
      }
      else
      {
        buffer[(BLOCK_SIZE - 1) * number_of_blocks + i] = buffer_block[i];
        text_len = text_len + 1;
      }
    }
    if (new_line_found == 0)
    {
      number_of_blocks = number_of_blocks + 1;

      new_buffer = (char*) realloc(buffer, (number_of_blocks + 1) * (BLOCK_SIZE - 1) * sizeof(char));

      if (new_buffer == NULL)
      {
        free(buffer_block);
        free(buffer);
        printf("Zu wenig Speicher vorhanden!\n");
        *return_value = 3;
        return new_buffer;
      }
      else
      {
        buffer = new_buffer;
      }
    }
    else
    {
      free(buffer_block);
      buffer = (char*) realloc(buffer, (text_len + 1) * sizeof(char));
      buffer[text_len] = '\0';
      return buffer;
    }

  }
  return NULL;

}

int getNumberOfSentences(char* user_input)
{
  int number_of_sentences = 0;

  for (int i = 0; user_input[i] != '\0'; i++)
  {
    if ((user_input[i] == '.') || (user_input[i] == '!') || (user_input[i] == '?'))
    {
      number_of_sentences = number_of_sentences + 1;
    }
  }
  return number_of_sentences;
}



int getLength(char* string)
{
  int string_length = 0;
  for (int i = 0; string[i] != '\0'; i++)
  {
    string_length = string_length + 1;
  }
  return string_length;
}


int lastSentenceSymbolMissing(char* user_input)
{
  int last_simbol_index = 0;

  for (int i = 0; user_input[i] != '\0'; i++)
  {
    if ((user_input[i] == '.') || (user_input[i] == '!') || (user_input[i] == '?'))
    {
      last_simbol_index = i;
    }
  }
  for (int i = last_simbol_index + 1; user_input[i] != '\0'; i++)
  {
    if (user_input[i] != ' ')
    {
      return 1;
    }
  }
  return 0;
}





char** getSentences(char* user_input, int* return_value)
{

  int number_of_sentences = getNumberOfSentences(user_input);

  char **sentences = malloc((number_of_sentences + 1) * sizeof(char*));

  if (sentences == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_value = 3;
    free(user_input);
    return NULL;
  }

  int start_index = 0;
  int stop_index = 0;
  int length = 0;

  int index = 0;

  for (int sentence = 0; sentence < number_of_sentences; sentence++)
  {
    length = 0;

    while(user_input[index] == ' ')
    {
      index = index + 1;
    }
    start_index = index;
    while (user_input[index] != '.' && user_input[index] != '!' && user_input[index] != '?')
    {
      length = length + 1;
      index = index + 1;
    }
    length = length + 1;
    stop_index = index;
    index = index + 1;

    sentences[sentence] = (char*)malloc((length + 1) * sizeof(char));

    if (sentences[sentence] == NULL)
    {
      printf("Zu wenig Speicher vorhanden!\n");
      *return_value = 3;
      for (int i = sentence - 1; i >= 0; i--)
      {
        free(sentences[i]);
      }
      free(sentences);
      free(user_input);
      return NULL;
    }

    for (int i = 0; i < length; i++)
    {
      sentences[sentence][i] = user_input[start_index + i];
    }
    sentences[sentence][length] = '\0';

  }

  sentences[number_of_sentences] = NULL;

  return sentences;

}

char getCommandFromUser()
{
  printf("\n");
  printf("Wählen Sie eine Option:\n");
  printf("  t: Text-Statistik\n");
  printf("  s: Zwei Sätze vertauschen\n");
  printf("  c: Zwei Sätze zusammenketten\n");
  printf("  p: Text mit Zeilennummer ausgeben\n");
  printf("  o: Text ohne Zeilennummer ausgeben\n");
  printf("  e: Programm beenden\n");
  printf("\n");
  
  char command = 'x';

  while (command != 't' && command != 's' && command != 'c' && command != 'p'
        && command != 'o' && command != 'e')
  {
    printf("Ihre Wahl: ");
    command = getchar();

    while (getchar() != '\n');
  }

  return command;

}

void printTextWithoutNumber(char** sentences, int number_of_sentences)
{
  printf("Text:\n");
  for (int i = 0; i < number_of_sentences; i++)
  {
    printf("%s ", sentences[i]);
  }
  printf("\n");
}

void printTextWithNumber(char** sentences, int number_of_sentences)
{
  printf(" Nr. | Satz\n");
  printf("-----+----------------------------------------\n");
  for (int i = 0; i < number_of_sentences; i++)
  {
    printf("%4d | %s\n", i + 1, sentences[i]);
  }
}

void freeMemory(char* user_input, char** sentences, int number_of_sentences)
{
  free(user_input);

  for (int i = 0; i < number_of_sentences; i++)
  {
    free(sentences[i]);
  }
  free(sentences);
}

void printTextStatistic(char** sentences, int number_of_sentences)
{
  int number_of_symbols = 0;
  int number_of_letters = 0;
  int number_of_uppercase_letters = 0;
  int number_of_lovercase_letters = 0;

  for (int i = 0; i < number_of_sentences; i++)
  {
    int j = 0;
    while (sentences[i][j] != '\0')
    {
      if (sentences[i][j] != ' ')
      {
        number_of_symbols = number_of_symbols + 1;
      }

      if (sentences[i][j] >= 'a' && sentences[i][j] <= 'z')
      {
        number_of_lovercase_letters = number_of_lovercase_letters + 1;
        number_of_letters = number_of_letters + 1;
      }
      if (sentences[i][j] >= 'A' && sentences[i][j] <= 'Z')
      {
        number_of_uppercase_letters = number_of_uppercase_letters + 1;
        number_of_letters = number_of_letters + 1;
      }
      j = j + 1;
    }
  }

  printf("Text Statistik:\n");
  printf("%d Sätze, %d Zeichen\n", number_of_sentences, number_of_symbols);
  printf("%d Buchstaben (%d Großbuchstaben, %d Kleinbuchstaben)\n", number_of_letters,
        number_of_uppercase_letters, number_of_lovercase_letters);
}


void switchTwoSentences(char*** sentences, int number_of_sentences)
{
  if (number_of_sentences == 1)
  {
    printf("Nur noch ein Satz vorhanden!\n");
    return;
  }

  int num1 = 0;
  int num2 = 0;

  int num1_not_accepted = 1;
  int num2_not_accepted = 1;

  while (num1_not_accepted)
  {
    printf("Nummer des ersten Satzes: ");
    scanf("%d", &num1);

    if (num1 <= number_of_sentences && num1 >= 1)
    {
      num1_not_accepted = 0;
    }
    while (getchar() != '\n');



  }

  while (num2_not_accepted)
  {
    printf("Nummer des zweiten Satzes: ");
    scanf("%d", &num2);

    if (num2 <= number_of_sentences && num2 >= 1 && num2 != num1)
    {
      num2_not_accepted = 0;
    }
    while (getchar() != '\n');
  }

  char *sentence_holder = (*sentences)[num1 - 1];
  (*sentences)[num1 - 1] = (*sentences)[num2 - 1];
  (*sentences)[num2 - 1] = sentence_holder;
}

void concatenateTwoSentences(char*** sentences, int* number_of_sentences, int* return_value)
{
  if (*number_of_sentences == 1)
  {
    printf("Nur noch ein Satz vorhanden!\n");
    return;
  }

  int num1 = 0;
  int num2 = 0;

  int num1_not_accepted = 1;
  int num2_not_accepted = 1;

  while (num1_not_accepted)
  {
    printf("Nummer des ersten Satzes: ");
    scanf("%d", &num1);


    if (num1 <= *number_of_sentences && num1 >= 1)
    {
      num1_not_accepted = 0;
    }
    while (getchar() != '\n');

  }

  while (num2_not_accepted)
  {
    printf("Nummer des zweiten Satzes: ");
    scanf("%d", &num2);

    if (num2 <= *number_of_sentences && num2 >= 1 && num2 != num1)
    {
      num2_not_accepted = 0;
    }
    while (getchar() != '\n');
  }

  int len1 = getLength((*sentences)[num1-1]);
  int len2 = getLength((*sentences)[num2-1]);

  char *new_sentence_holder = (char*)realloc((*sentences)[num1-1], (len1+4+len2+1)*sizeof(char));
  if (new_sentence_holder == NULL)
  {
    printf("Zu wenig Speicher vorhanden!\n");
    *return_value = 3;
    return;
  }

  (*sentences)[num1-1] = new_sentence_holder;

  (*sentences)[num1-1][len1-1] = ' ';
  (*sentences)[num1-1][len1] = 'u';
  (*sentences)[num1-1][len1+1] = 'n';
  (*sentences)[num1-1][len1+2] = 'd';
  (*sentences)[num1-1][len1+3] = ' ';

  for (int i = 0; i < len2; i++)
  {
    (*sentences)[num1-1][len1+4+i] = (*sentences)[num2-1][i];
  }

  (*sentences)[num1-1][len1+4+len2] = '\0';

  char *ptr = (*sentences)[num2-1];

  //memmove(&sentences[num2-1], &sentences[num2], (*number_of_sentences-num2+2)*sizeof(char*));


  for (int i = num2-1; i < *number_of_sentences; i++)
  {
    (*sentences)[i] = (*sentences)[i+1];
  }

  free(ptr);

  //free(sentences[*number_of_sentences+1]);

  *number_of_sentences = *number_of_sentences - 1;

  (*sentences) = (char**)realloc((*sentences), (*number_of_sentences+1)*sizeof(char*));

}



