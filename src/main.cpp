#include "ruratae.hpp"
#include <stdio.h>

int main(int argc, char* argv[])
{
  ruratae::instrument instr(10, 10);
	int a = instr.create_particle(ruratae::particle_params());
  int b = instr.create_particle(ruratae::particle_params(
		ruratae::vec3(1, 0, 0), ruratae::vec3(0), 1.0f));
	int c = instr.create_spring(ruratae::spring_params(a, b, 3000000, 2000000));
  for (int i = 0; i < 96000; i++)
  {
    float out;
    ruratae::vec3 xp = instr.get_particle_position(b);
    printf("%f\n", xp.x);
    instr.process(&out, 1);
  }
  return 0;
}