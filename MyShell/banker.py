"""Name: Mohan Thapa 
   CSC 360/ 660, Operating System 
   programmong Project #5: Implememtation of Banker's Algorithm
   October 9, 2023 
   Instructor : Dr. Siming Liu
   """








class BankersAlgorithm:
    def __init__(self, input_file):
        self.input_file = input_file
        self.n_processes = 0
        self.n_resources = 0
        self.allocation_matrix = []
        self.max_matrix = []
        self.available_vector = []
        self.request_process = 0
        self.request_vector = []
        self.need_matrix = []

    def read_input_file(self):
        try:
            with open(self.input_file, 'r') as file:
                self.n_processes = int(file.readline())
                self.n_resources = int(file.readline())
                

                # Read allocation matrix
                for _ in range(self.n_processes):
                    allocation_row = list(map(int, file.readline().split()))
                    self.allocation_matrix.append(allocation_row)

                # Read max matrix
                for _ in range(self.n_processes):
                    max_row = list(map(int, file.readline().split()))
                    self.max_matrix.append(max_row)

                # Read need matrix
                for process in range (len(self.max_matrix)):
                    need_row = [self.max_matrix[process][resource] - self.allocation_matrix[process][resource] for resource in range(len(self.max_matrix[process])) ]
                    self.need_matrix.append(need_row)

                # Read available vector
                self.available_vector = list(map(int, file.readline().split()))

                # Read request process and vector
                request_line = file.readline().strip().split(':')
                self.request_process = int(request_line[0])
                self.request_vector = list(map(int, request_line[1].split()))

            return True
                
        

        except FileNotFoundError:
            print(f"Error: File '{self.input_file}' not found.")
            return False
        

    

        
    # Safety Algorithm
    def check_safety(self):
        work_vector = self.available_vector[:]
        finish = [False] * self.n_processes
        safe_sequence = []

        while True:
            for process in range(self.n_processes):
                if not finish[process] and all(
                        self.need_matrix[process][resource] <= work_vector[resource] for resource in range(self.n_resources)):
                    work_vector = [work_vector[resource] + self.allocation_matrix[process][resource]
                                   for resource in range(self.n_resources)]
                    finish[process] = True
                    safe_sequence.append(process)

            if all(finish):
                return True, safe_sequence

            if not any(finish):
                return False, []

    def request_resources(self):
        if self.request_process < 0 or self.request_process >= self.n_processes:
            print("Error: Invalid process ID in request.")
            return

        if any(self.request_vector[resource] > self.max_matrix[self.request_process][resource]
               for resource in range(self.n_resources)):
            print("Error: Request exceeds maximum claim.")
            return

        if any(self.request_vector[resource] > self.available_vector[resource]
               for resource in range(self.n_resources)):
            print("Error: Resources not available for the request.")
            return

        self.available_vector = [self.available_vector[resource] - self.request_vector[resource]
                                 for resource in range(self.n_resources)]
        self.allocation_matrix[self.request_process] = [self.allocation_matrix[self.request_process][resource]
                                                        + self.request_vector[resource]
                                                        for resource in range(self.n_resources)]

        safe, sequence = self.check_safety()

        if safe:
            print("\nTHE SYSTEM IS IN SAFE STATE!")
            print("\nThe Request Vector is...")
            print(f"    {' '.join(chr(65 + i) for i in range(len(self.request_vector))):^}")
            print(f"{self.request_process} : {' '.join(map(str, self.request_vector)):^}")  
            print("\nTHE REQUEST CAN BE GRANTED!")
            print("\nThe Available Vector is...")
            print(f"{' '.join(chr(65 + i) for i in range(len(self.available_vector))):^}")
            print(f"{' '.join(map(str, self.available_vector)):^}")
            print("\nSafe sequence:", sequence)
        else:
            print("\nTHE SYSTEM IS NOT IN SAFE STATE!")
            print(f"    {' '.join(chr(65 + i) for i in range(len(self.request_vector))):^}")
            print(f"{self.request_process} : {' '.join(map(str, self.request_vector)):^}")  
            print("\nTHE REQUEST CAN BE GRANTED!")
            print("THE REQUEST CANNOT BE GRANTED!")

    def print_matrices(self):
    
        print(f"\nThere are {self.n_processes} processes in the system.")
        print(f"\nThere are {self.n_resources} resource types.")
        print("\nThe Allocation Matrix is...")
        print("   A B C D")
        self.print_matrix(self.allocation_matrix,)
        print("\nThe Max Matrix is...")
        print("   A B C D")
        self.print_matrix(self.max_matrix)
        print("\nThe Need Matrix is...")
        print("   A B C D")
        self.print_matrix(self.need_matrix)
        print("\nThe Available Vector is...")
        print(f"{' '.join(chr(65 + i) for i in range(len(self.available_vector))):^}")
        print(f"{' '.join(map(str, self.available_vector)):^}")
        

    @staticmethod
    def print_matrix(matrix):
        for i, row in enumerate(matrix):
            print(f"{i}: {' '.join(map(str, row))}")


import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py <input_filename>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    banker = BankersAlgorithm(input_file)
    

    if banker.read_input_file():
        banker.print_matrices()
        banker.request_resources()

if __name__ == "__main__":
    main()
