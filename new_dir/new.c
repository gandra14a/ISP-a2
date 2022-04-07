#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5




char* getUserInput2()
{
  char* buffer = (char*)malloc(MAX_SIZE * sizeof(char));
  if(buffer == NULL)
    return NULL;

  fgets(buffer, MAX_SIZE, stdin);
  buffer[strcspn(buffer, "\n")] = 0; // NICHT FÜR A2 VERWENDEN - ist in string.h
  return buffer;
}

char* redoubleAndGetUserInput(char* user_input)
{
  char* new_buffer = NULL; //(char*)realloc(user_input, MAX_SIZE * 2);
  if(new_buffer == NULL)
  {
    return NULL;
  }


  fgets(new_buffer, MAX_SIZE*2, stdin);
  new_buffer[strcspn(new_buffer, "\n")] = 0; // NICHT FÜR A2 VERWENDEN - ist in string.h
  return new_buffer;
}


int main()
{
  // 1) stack
  //char* user_input = getUserInput();


  // 2) HEAP malloc
  char* user_input = getUserInput2();
  if(user_input == NULL)
    return -1;
  printf("User Input after malloc : [%s]\n", user_input);
  // free(user_input)

  // 3) REALLOC
  user_input = redoubleAndGetUserInput(user_input);
  if(user_input == NULL)
    return -1;
  printf("User Input after realloc: [%s]\n", user_input);

  free(user_input);

  return 0;
}