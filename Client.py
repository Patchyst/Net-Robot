import socket
import curses

ESP8266_ADDR = "" # local IP of esp8266
ESP8266_PORT = 8888

movement_keys = [curses.KEY_RIGHT, curses.KEY_LEFT, curses.KEY_UP, curses.KEY_DOWN ]
is_manual = True

def control_init():
    curses.noecho()
    curses.cbreak()

def control_fini():
    curses.nocbreak()
    curses.echo()

def move(right_servo, left_servo, control_sock):
    control_sock.send(right_servo.encode())
    control_sock.send(left_servo.encode())

if __name__ == "__main__": # Curses does not always fully restore cmd
    try:
        control_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        control_socket.connect((ESP8266_ADDR, ESP8266_PORT))
    except Exception as e:
        print("FAILURE TO CONNECT TO ESP8266 with code "+str(e))
        exit(1)
    prompt = "Connected to ESP8266 Control Service ("+ESP8266_ADDR+":"+str(ESP8266_PORT)+")"
    print(prompt)
    if is_manual:
        print("\tOperating in manual mode")
    input("\tPress any key to begin...")
    terminal = curses.initscr()
    control_init()
    terminal.keypad(True)

    while is_manual:
        in_buff = terminal.getch()
        if in_buff == ord('q'):
            move("90\n", "90\n", control_socket)
            control_fini()
            terminal.keypad(0)
            is_manaul = False
        elif in_buff in movement_keys:
            if(in_buff == curses.KEY_RIGHT):
                move("180\n", "90\n", control_socket)
            elif(in_buff == curses.KEY_LEFT):
                move("0\n", "90\n", control_socket)
            elif(in_buff == curses.KEY_UP):
                move("0\n", "180\n", control_socket)
            elif(in_buff == curses.KEY_DOWN):
                move("180\n", "0\n", control_socket)
        else:
            move("90\n", "90\n", control_socket)
