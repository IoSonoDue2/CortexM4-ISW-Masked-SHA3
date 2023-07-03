from tqdm import tqdm
import serial
import random
import string
import time
import hashlib


MODE = "sha3_256" 
SERIAL_INTERFACE ="/dev/ttyUSB0"

ITERATIONS = 20
INITIAL_LEN= 9



ser = serial.Serial(SERIAL_INTERFACE, 115200, timeout=1)


def generate_random_string(length):
    letters = string.ascii_letters  # a-z e A-Z
    result_str = ''.join(random.choice(letters) for _ in range(length))
    return result_str


def calculate_sha3_256_hash(input_string):
    # Calcola l'hash
    hash_object = hashlib.sha3_256(input_string.encode())
    digest_bytes = hash_object.digest() 
    binary_representation = ''.join(f'{byte:08b}' for byte in digest_bytes)
    
    return binary_representation

def calculate_sha3_512_hash(input_string):
    # Calcola l'hash
    hash_object = hashlib.sha3_512(input_string.encode())
    digest_bytes = hash_object.digest() 
    binary_representation = ''.join(f'{byte:08b}' for byte in digest_bytes)
    
    return binary_representation

def divide_string(input_string, chunk_size=256):

    string_parts = []
    

    while len(input_string)>chunk_size:

        string_part = input_string[:chunk_size]
        

        string_parts.append(string_part)

        input_string = input_string[chunk_size:]

    return string_parts,input_string


def send_string_packet(ser, str):

    packets, last = divide_string(str, 256)

    for packet in packets:

        
        ser.write(packet.encode())      
        time.sleep(0.05)

    last_enc=last.encode()
    ser.write(last_enc)



def test_command(str):
    flag=False

    response = b""
    
    send_string_packet(ser,str)
    time.sleep(0.05)
    i=0;
    start_time = time.time()
    ser.write(b'\r')

    


    while True:

        byte = ser.read(1)

        if(MODE=="sha3_256" and i==256):
            break
        elif(i==512):
            break



        if(byte==b'0' or byte==b'1' ):
            response += byte
            i+=1

            
    

    end_time = time.time()
    

    if(MODE=="sha3_256"):
        flag=response.decode()==calculate_sha3_256_hash(str)
    else:
        flag=response.decode()==calculate_sha3_512_hash(str)


    elapsed_time = end_time - start_time

    return  elapsed_time,flag





ser.write(MODE.encode()+b'\r')
time.sleep(2)
# for i in range (9):
#     ser.read(1)

           
failure_count = 0 

with open(MODE+'_results_7S.txt', 'w') as f:

    for k in tqdm(range(1,101), desc='Overall progress', ncols=75, position=0):

        for i in tqdm(range(ITERATIONS), desc=f'Processing length {INITIAL_LEN*5*k}', ncols=75, position=1, leave=False):
            str = generate_random_string(INITIAL_LEN*5*k)
            elapsed_time, success = test_command(str)

            elapsed_time = round(elapsed_time, 5)

            f.write("Length: {} ".format(INITIAL_LEN*5*k))
            f.write("Elapsed_time: {} ".format(elapsed_time))
            f.write("Success: {}\n".format(success))

            if not success:
                failure_count += 1 
                print(f"Failure #{failure_count} at iteration {i} of length {INITIAL_LEN*5*k}")

ser.close()

print(f"Total number of failures: {failure_count}")










