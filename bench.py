import subprocess
from multiprocessing import Pool, cpu_count
from datetime import datetime

def run_c_program(arg):
    command = ['./bin/command', *arg] 
    subprocess.call(command, stdout=subprocess.PIPE)

if __name__ == '__main__':
    create_arguments = [['create', '23:59'] for _ in range(10)]

    update_arguments = [['update', '0', '-v', '50'] for _ in range(10)]
    get_arguments = [['get', '0'] for _ in range(10)]

    start = datetime.now()
    with Pool(5) as pool:
        pool.map(run_c_program, create_arguments)
    end = datetime.now()
    delta = end - start
    print(f"create: {delta.total_seconds()*1000 /10:.2f}ms")

    start = datetime.now()
    with Pool(5) as pool:
        pool.map(run_c_program, update_arguments)
    end = datetime.now()

    delta = end - start
    print(f"update: {delta.total_seconds()*1000 /10:.2f}ms")

    start = datetime.now()
    with Pool(5) as pool:
        pool.map(run_c_program, get_arguments)
    end = datetime.now()
    delta = end - start
    print(f"get: {delta.total_seconds()*1000 /10:.2f}ms")