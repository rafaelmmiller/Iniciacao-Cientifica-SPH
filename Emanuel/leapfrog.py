# -*- coding: utf-8 -*-
'''
Tentative Leapfrog Method
v0.1 - 18/11/16
By Emanuel Lima - emanuel.lima.sousa@usp.br
On Python 3
Based on Paper "The Leapfrog method and other sympletic algorithms for
integrating Newton's laws of motion" by Peter Young

TODO: Recursion
      Time mesure
'''
######################################################################################
PROMPT = "Digite (R) para recursivo, (I) para iterativo ou (E) para encerrar: "
WELCOME = "Tentative Leapfrog v0.1\n"
INITIAL = "Digite as condições iniciais X e V separadas por um espaço em branco: "
N_ITER = "Digite o número de iterações desejadas: "
FINAL_X = "O valor final de x é "
FINAL_Y = "O valor final de v é "
EXIT = "\nFim\n"
N_H = "Digite o intervalo de tempo h: "
######################################################################################
FORCE = 10
######################################################################################
def main():
    print(WELCOME)
    user = input(PROMPT)
    #User choices
    if user == "R" or user == "r":
        #Initial conditions
        ini_conditions_str = []
        ini_conditions = []
        condition = input(INITIAL)
        ini_conditions_str = condition.split()
        ini_conditions = [float(i) for i in ini_conditions_str]
        #Number of iterations
        n = int(input(N_ITER))
        #Interval
        h = float(input(N_H))
        x, v = recursive(ini_conditions, n, h)
        print()
        print(FINAL_X, x)
        print(FINAL_Y, v)
        print()
        main()

    elif user == "I" or user == "i":
        #Initial conditions
        ini_conditions_str = []
        ini_conditions = []
        condition = input(INITIAL)
        ini_conditions_str = condition.split()
        ini_conditions = [float(i) for i in ini_conditions_str]
        #Number of iterations
        n = int(input(N_ITER))
        #Interval
        h = float(input(N_H))
        x, v = iterative(ini_conditions, n, h)
        print()
        print(FINAL_X, x)
        print(FINAL_Y, v)
        print()
        main()

    elif user == "E" or user == "e":
        print(EXIT)
        return None

    else:
        print("\nOpção inválida. Tente novamente.\n")
        main()

def recursive(ini_conditions, n, h):
    '''
    (list, int, float) -> float, float
    Receives a list of initial conditions, the number of iterations
    and the time step and returns the final position and velocity recursively.
    '''
    print("\nAinda não implementado!")
    return 0.0, 0.0

def iterative(ini_conditions, n, h):
    '''
    (list, int, float) -> float, float
    Receives a list of initial conditions, the number of iterations
    and the time step and returns the final position and velocity iteractively.
    '''
    vel_final = ini_conditions[1] + 0.5 * h * FORCE
    pos_final = ini_conditions[0] + h * vel_final
    vel_final = vel_final + h * FORCE
    while n > 0:
        pos_final = pos_final + h * vel_final
        vel_final = vel_final + h * FORCE
        n -= 1
    return pos_final, vel_final

#################################################################################
if __name__ == '__main__':
    main()
