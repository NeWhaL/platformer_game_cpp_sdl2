#include "../include/application.h"

int main(int argc, char* argv[]) {
  init_application();
  application();
  de_init_application(0);
  return 0;
}