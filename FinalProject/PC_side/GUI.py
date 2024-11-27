import PySimpleGUI as sg
import serial as ser
import sys, glob
import time
import serial.tools.list_ports
from tkinter import *
from tkinter.colorchooser import askcolor
import mouse
import os
from os import path
import threading
import binascii
import pyautogui


def process_counter_data(data):
    # Remove non-numeric characters (for example, null bytes)
    cleaned_data = ''.join(filter(str.isdigit, data))
    return int(cleaned_data) if cleaned_data else 0



def File_Translator(data_from_file):
    # Define the operation codes for each command
    operation_codes = {
        'inc_lcd': '01',  # Increment LCD command
        'set_delay': '04',  # Set delay command
        'dec_lcd': '02',  # Decrement LCD command
        'rra_lcd': '03',  # Rotate right LCD command
        'clear_lcd': '05',  # Clear LCD command
        'stepper_deg': '06',  # Stepper motor degrees command
        'stepper_scan': '07',  # Stepper motor scan command
        'sleep': '08'  # Sleep command
    }

    # Prepare an empty list to store translated lines
    translated_lines = []

    # Decode byte data to string and split into lines
    data_str = data_from_file.decode('utf-8').strip()
    lines = data_str.split('\r\n')

    for line in lines:
        # Split command and data
        parts = line.split()
        command = parts[0]

        if command in operation_codes:
            op_code = operation_codes[command]
            if len(parts) > 1:
                # Data part
                data = parts[1]
                if ',' in data:
                    # For commands like stepper_scan which have multiple data points
                    data_values = data.split(',')
                    formatted_data = ''.join([f'{int(d):02X}' for d in data_values])
                else:
                    # Single data point
                    formatted_data = f'{int(data):02X}'

                translated_line = op_code + formatted_data
            else:
                # No data part, just the operation code
                translated_line = op_code

            # Add translated line to list
            translated_lines.append(translated_line)

    # Write the translated lines to a new file
    translated_file_path = 'translated_file.txt'
    with open(translated_file_path, 'w') as f:
        for line in translated_lines:
            f.write(line + '\n')

    return translated_file_path  # Return the file path instead of the file object



class Paint(object):
    size = 5.0  # Default brush size
    color = 'black' # Default brush color

    def __init__(self):
        self.root = Tk() # Initialize the main Tkinter window

        # Create buttons for various actions
        self.pen_btn = Button(self.root, text='pen', command=self.activate_pen)

        self.brush_btn = Button(self.root, text='Back', command=self.close_application)
        self.brush_btn.grid(row=0, column=1)

        self.color_btn = Button(self.root, text='color', command=self.pick_color)

        self.eraser_btn = Button(self.root, text='eraser', command=self.use_eraser)

        # Slider for selecting brush size
        self.size_selector = Scale(self.root, from_=6, to=10, orient=HORIZONTAL)
        self.size_selector.set(6)

        # Canvas for drawing
        self.c = Canvas(self.root, bg='grey', width=1700, height=1700)
        self.c.grid(row=1, columnspan=5)

        # Initialize state and event bindings
        self.initialize_state()
        self.root.mainloop()

    def initialize_state(self):
        # Initialize the drawing state
        self.previous_x  = None
        self.previous_y = None
        self.brush_size  = self.size_selector.get()
        self.color = self.color
        self.eraser_mode   = False
        self.current_tool    = self.pen_btn
        self.c.bind('<Motion>', self.paint) # Bind mouse motion for drawing
        self.c.bind('<ButtonRelease-1>', self.reset) # Bind mouse button release for resetting state

    def activate_pen(self):
        # Activate the pen tool
        self.switch_tool(self.pen_btn)

    def activate_eraser(self):
        # Activate the eraser tool
        self.switch_tool(self.brush_btn)

    def pick_color(self):
        # Open color chooser dialog and set the selected color
        self.eraser_mode = False
        self.color = askcolor(color=self.color)[1]

    def use_eraser(self):
        # Activate the eraser tool
        self.switch_tool(self.eraser_btn, eraser_mode=True)

    def switch_tool(self, some_button, eraser_mode=False):
        # Switch between tools
        self.current_tool.config(relief=RAISED) # Reset the current tool button
        some_button.config(relief=SUNKEN) # Set the new tool button as active
        self.current_tool = some_button
        self.eraser_mode = eraser_mode # Update the mode to eraser if specified

    def paint(self, event):
        global state
        # Draw lines on the canvas based on the selected tool and mode
        if state == 0 and self.previous_x  and self.previous_y:  # Drawing with pen
            self.c.create_line(self.previous_x , self.previous_y, event.x, event.y,width=self.brush_size , fill=self.color,capstyle=ROUND, smooth=TRUE, splinesteps=36)
        elif state == 1 and self.previous_x  and self.previous_y:  # Erasing
            self.c.create_line(self.previous_x , self.previous_y, event.x, event.y,width=self.brush_size , fill='grey',capstyle=ROUND, smooth=TRUE, splinesteps=36)
        elif state == 2:  # Neutral state
            pass
        else:
            pass

        # Update the previous position for the next line
        self.previous_x  = event.x
        self.previous_y = event.y

    def reset(self, event):
        # Reset the previous position when the mouse button is released
        self.previous_x , self.previous_y = None, None

    def close_application(self):
        # Close the Tkinter window
        global paint_active
        paint_active = 0
        self.root.destroy()


def toggle_window_layout(layout_index, window):
    # Toggle visibility of different layout columns in the GUI
    for i in range(1, 7):
        visibility = i == layout_index
        window[f'COL{i}'].update(visible=visibility)


def transmit_state(message=None, use_file=False):
    # Transmit a message to the serial port
    s.reset_output_buffer() # Clear the output buffer
    if use_file:
        bytesMenu = message # Use file data directly
    else:
        bytesMenu = bytes(message, 'ascii') # Convert message to bytes
    s.write(bytesMenu) # Send the message through the serial port


def fetch_data(state, size):
    # Fetch data from the serial port based on the state
    while True:
        while s.in_waiting > 0:  # Check if there's data available in the input buffer
            if state == 'Painter':
                message = s.read(size=size)  # Read a specific number of bytes (6)
                message = binascii.hexlify(message).decode('utf-8') # Convert to hexadecimal string
                temp_messege = "".join([message[x:x + 2] for x in range(0, len(message), 2)][::-1]) # Reverse byte order
                Rx_messege = [temp_messege[i:i + 4] for i in range(0, len(temp_messege), 4)] # Split into chunks
            elif state == 'script':
                Rx_messege = s.read(size=size).decode('utf-8')  # Read and decode data as UTF-8 string
            else:
                Rx_messege = s.readline().decode('utf-8') # Read a line of text
            return Rx_messege



def retrieve_joystick_telemetry():
    # Retrieve and process joystick telemetry data
    telemetry = fetch_data('Painter', 4)
    x_val = int(telemetry[0], 16)# Convert from hexadecimal to integer
    y_val = int(telemetry[1], 16)# Convert from hexadecimal to integer

    # Check if values are out of expected range and reverse byte order if needed
    if x_val > 1024 or y_val > 1024:
        telemetry[0] = telemetry[0][::-1]
        telemetry[1] = telemetry[1][::-1]
        x_val = int(telemetry[0], 16)
        y_val = int(telemetry[1], 16)

    return x_val, y_val


def initialize_painter(window):
    # Initialize and start the Paint class for painting application
    Paint()


def create_gui():
    sg.theme('DarkAmber')  # Change to your preferred theme
    get_Ack = ''
    # Define a new layout for the main menu
    main_screen   = [
        [sg.Text("Guy&Liav final DCS project", size=(50, 2), justification='center', font='Garamond 30')],
        [sg.Text("Select an option  (please start with Stepper Motor Calibration):", size=(50, 1), font='Garamond 25')],
        [sg.Button("1) Manual control of motor-based machine", key='s1_manual_stepper', size=(40, 4), font='Garamond 15'),
         sg.Button("2) Joystick based PC painter", key='s2_painter', size=(40, 4), font='Garamond 15')],
        [sg.Button("3) Stepper Motor Calibration", key='s3_calibaration', size=(40, 4), font='Garamond 15'),
         sg.Button("4) Script Mode", key='s4_script_mode', size=(40, 4), font='Garamond 15')]
    ]

    # Define layout for manual control screen
    manual_screen = [[sg.Text("Manual control of motor-based machine", size=(50, 2), justification='center', font='Garamond 20')],
        [sg.Text("please rotate the pointer to 0 deg:", size=(75, 1),font='Garamond 20')],
        [sg.Frame("",layout=[[sg.Button("Rotate Stepper Motor To 0 Deg", key='motor_rotation', size=(75, 2), font='Garamond 15')],
        [sg.Button("Stop Now", key='stop_motor', size=(75, 2), font='Garamond 15')],
        [sg.Button("Joystick Control", key='control_to_joy', size=(75, 2), font='Garamond 15')]],
        title_color='blue',relief=sg.RELIEF_SUNKEN)],
        [sg.Text("", size=(50, 2))],  # Spacer
        [sg.Button("Back", key='go_back', size=(10, 2), font='Garamond 15', pad=(0, 20))]]

    # Define layout for painter screen
    painter_screen = [
        [sg.Text("Joystick based PC painter", size=(35, 2), justification='center', font='Garamond 15')],
        [sg.Canvas(size=(1000, 1000), background_color='red', key='canvas')],
        [sg.Button("Back", key='go_back', size=(5, 1), font='Garamond 15', pad=(300, 180))]
    ]

    # Define layout for calibration screen
    calibration_screen = [
        [sg.Text("Stepper Motor Calibration", size=(35, 2), justification='center', font='Garamond 30')],
        [sg.Button("Start", key='motor_rotation', size=(75, 2), font='Garamond 20')],
         [sg.Button("Stop", key='stop_motor', size=(75, 2), font='Garamond 20')],
        [sg.Text("amount of moves: ", justification='center', font='Garamond 20'),
         sg.Text(" ", size=(35, 2), key="Counter", justification='center', font='Garamond 20')],
        [sg.Text("Phi: ", justification='center', font='Garamond 20'),
         sg.Text(" ", size=(35, 2), key="Phi", justification='center', font='Garamond 20')],
        [sg.Button("Back", key='go_back', size=(5, 1), font='Garamond 20')]
    ]

    # Define layout for file selection screen
    file_selection_screen = [[sg.Text("Upload folder -> Choose file -> Execute", size=(35, 2),  font='Garamond 17')],
                   [sg.Text("Folder"),sg.In(size=(45, 1), enable_events=True, key='folder_of_file'), sg.FolderBrowse()],
                   [sg.Listbox(values=[], enable_events=True, size=(60, 23), key="list_of_files")],
                   [sg.Button('Back', key='go_back', size=(5, 1), font='Garamond 15'),
                    sg.Button('Choose', key='choose_file', size=(7, 1), font='Garamond 15')],
                    [sg.Text(' ', key='ack_from_MSP', size=(35, 1), font='Garamond 10')]]

    # Define layout for file details screen
    file_details_screen = [
        [sg.Text("Content", size=(35, 1), justification='center', font='Garamond 25')],
        [sg.Text(size=(42, 1), key="fileH", font='Garamond 11')],
        [sg.Multiline(size=(70, 15), key="file_content")],
        [sg.HSeparator()],
        [sg.Text("Files to execute", size=(35, 1), justification='center', font='Garamond 25')],
        [sg.Listbox(values=[], enable_events=True, size=(70, 4), key="execute_file")],
        [sg.Button('Execute', key='execute', size=(17, 1), font='Garamond 15'),
        sg.Button('Delete', key='_Clear_', size=(19, 1), font='Garamond 15')]
    ]

    # Define layout for script execution screen
    script_execution_screen = [
        [sg.Text("Deg of pointer: ", justification='center', font='Garamond 15'),
         sg.Text(" ", size=(35, 2), key="Degree", justification='center', font='Garamond 15')],
        [sg.Button('send to MSP430', key='Run', size=(60, 2), font='Garamond 15')],
        [sg.Button("Back", key='go_back_script', size=(5, 1), font='Garamond 15')]
    ]

    # Define layout for script mode screen
    script_screen = [[sg.Column(file_selection_screen),
                      sg.VSeparator(),
                      sg.Column(file_details_screen)]]

    # Define the overall layout combining all individual screens
    layout = [[sg.Column(main_screen, key='COL1', ),
               sg.Column(manual_screen, key='COL2', visible=False),
               sg.Column(painter_screen, key='COL3', visible=False),
               sg.Column(calibration_screen, key='COL4', visible=False),
               sg.Column(script_screen, key='COL5', visible=False),
               sg.Column(script_execution_screen, key='COL6', visible=False)]]

    global window
    # Create the PySimpleGUI window
    window = sg.Window(title='Final project 2024: Control System of motor-based machine', element_justification='c', layout=layout,
                       size=(2200, 1200))

    # Initialize global variables
    files_to_MSP  = []
    TEMP_LIST  = []
    f_name  = ''
    file_path = ''
    global paint_active

    # Event loop to handle user interactions
    while True:
        event, values = window.read()
        if event == "s1_manual_stepper": # Handle the event when the user selects the manual control option
            transmit_state('m')  # Set the system to manual control mode
            toggle_window_layout(2, window)  # Switch the window layout to show the manual control interface
            while "go_back" not in event:  # Continuously process events while in the manual control layout
                event, values = window.read()  # Read the next event and values
                if event == "motor_rotation":  # Handle the event for rotating the stepper motor
                    transmit_state('A')  # Send command to rotate the motor
                elif event == "stop_motor": # Handle the event for stopping the stepper motor
                    transmit_state('M') # Send command to stop the motor
                elif event == "control_to_joy":  # Handle the event for switching to joystick control
                    transmit_state('J')  # Send command to switch control mode to joystick

        if event == "s2_painter":   # Handle the event when the user selects the joystick painter option
            global state
            paint_active = 1 # Set paint_active flag to indicate that painter mode is active
            transmit_state('P')   # Send command to switch to painter mode
            painter_thread = threading.Thread(target=initialize_painter, args=(window,))  # Start a new thread to initialize the painter (joystick control)
            painter_thread.start()

            # Initialize variables for joystick position tracking
            initial_time  = 1
            while paint_active:
                try:
                    Vx, Vy = retrieve_joystick_telemetry()  # Retrieve joystick telemetry data
                except:
                    continue

                # Handle joystick position and update cursor position accordingly
                if Vx == 1000 and Vy == 1000: #transition between tools
                    state = (state + 1) % 3
                elif initial_time : # Record initial joystick position
                    x_init, y_init = Vx, Vy
                    initial_time  = 0
                else:
                    dx, dy = Vx, Vy
                    curr_x, curr_y = mouse.get_position()  # read the cursor's current position
                    dx, dy = int(dx) - int(x_init), int(dy) - int(y_init)  # convert to int
                    mouse.move(curr_x - int(dx / 50), curr_y - int(dy / 50))  # move cursor to desired position
            toggle_window_layout(1, window)  # Return to the main menu layout

        # Handle the event when the user selects the calibration option
        if event == "s3_calibaration":
            transmit_state('C')  # Send command to start calibration mode
            toggle_window_layout(4, window) # Switch to the calibration layout
            while "go_back" not in event:
                event, values = window.read() # Read the next event and values
                if "motor_rotation" in event:
                    transmit_state('A')  # Send command to auto-rotate the motor
                elif "stop_motor" in event:
                    transmit_state('M')  # Send command to stop the motor
                    # Fetch and update the calibration data
                    counter = fetch_data('calib', 4)
                    window["Counter"].update(value=counter)
                    phi = int(counter.split('\x00')[0]) / 360
                    window["Phi"].update(value=str(round(phi, 4)) + "[degrees]")

        # Handle the event when the user selects the script mode option
        if event == "s4_script_mode":
            index_of_file = 0
            transmit_state('s')  # Send command to switch to script mode
            toggle_window_layout(5, window) # Switch to the script mode layout

        if event == 'folder_of_file': # Handle the event for selecting a folder containing script files
            selected_folder = values['folder_of_file']
            try:
                # Clear previous file content and file header information
                window["file_content"].update('')
                window["fileH"].update('')
                file_path = ''
                # List all files in the selected folder
                file_list = os.listdir(selected_folder)
            except Exception as e:
                file_list = []
            # Filter for text files only
            fnames = [f for f in file_list if os.path.isfile(os.path.join(selected_folder, f)) and f.lower().endswith(".txt")]
            window["list_of_files"].update(fnames) # Update the listbox with available text files

        # Handle the event for selecting a file from the list
        if event == 'list_of_files':
            try:
                # Construct the full path to the selected file
                file_path = os.path.join(values["folder_of_file"], values["list_of_files"][0])
                f_size = path.getsize(file_path)
                try:
                    # Read the content of the selected file
                    with open(file_path, "rt", encoding='utf-8') as f:
                        file_content = f.read()
                except Exception as e:
                    print("Error: ", e)
                # Update file header and content display
                f_name  = values["list_of_files"][0]
                window["fileH"].update(f"File name: {f_name } | Size: {f_size} Bytes")
                window["file_content"].update(file_content)
            except Exception as e:
                print("Error: ", e)
                window["file_content"].update('')

        if event == 'choose_file': # Handle the event for choosing a file to transmit
            transmit_state(f"{f_name }\n") # Send the file name to the device
            files_to_MSP .append(f"{f_name }")
            time.sleep(0.1)
            time.sleep(0.1)
            try:
                # Process and translate the selected file for transmission
                with open(file_path, "rb") as f:  # Open file in binary read mode
                    FileContent = f.read()
                    translated_file_path = File_Translator(FileContent)  # Call the translator function
                    # Read the translated file content to burn
                    with open(translated_file_path, "rb") as translated_file:
                        translated_content = translated_file.read()
                        FileContent = translated_content
                        transmit_state(translated_content + bytes('Z', 'utf-8'), use_file=False) # Send the translated file content to the device

            except Exception as e:
                print("Error: ", e)
            transmit_state(FileContent + bytes('Z', 'utf-8'), use_file=True) # Send the original file content
            time.sleep(0.5)
            # Determine the pointer state based on file index
            if (index_of_file == 0):
                ptr_state = 'W'
            elif (index_of_file == 1):
                ptr_state = 'X'
            elif (index_of_file == 2):
                ptr_state = 'Y'
            index_of_file += 1 # Increment file index and send pointer state to the device
            transmit_state(ptr_state)
            try:
                get_Ack = fetch_data('script', 3).rstrip('\x00')  # Fetch acknowledgment from the device
            except:
                print("error")
            if get_Ack == "FIN": # Update the list of files to be executed
                window.refresh()
            time.sleep(0.3)
            window['execute_file'].update(files_to_MSP )

        if event == 'execute_file': # Handle the event for selecting a file to execute
            file_to_MSP  = values["execute_file"][0] # Determine the index of the selected file
            file_index = files_to_MSP .index(file_to_MSP)  # for send state - 0,1,2
            if (file_index == 0):
                exe_state = 'T'
            elif (file_index == 1):
                exe_state = 'U'
            elif (file_index == 2):
                exe_state = 'V'

        if event == 'execute': # Handle the event for executing the selected file
            step_phi = 360 / 512  # Define the step size for pointer movement
            toggle_window_layout(6, window)
            window['folder_of_file'].update('"' + file_to_MSP  + '"' + " execute window")
            # Continuously process events while executing the script
            while "go_back_script" not in event:
                event, values = window.read()
                # Handle the event for running the script
                if event == 'Run':
                    time.sleep(0.5)
                    transmit_state(exe_state) # Send execution state to the device
                    time.sleep(0.6)
                    s.reset_input_buffer()
                    s.reset_output_buffer()
                    curr_counter = fetch_data('script', 3).rstrip('\x00') # Fetch and update the current counter value
                    while curr_counter != "FIN": # Update degree value based on counter
                        while 'F' not in curr_counter:
                            window.refresh()
                            curr_counter_cleaned = process_counter_data(curr_counter)
                            curr_phi = curr_counter_cleaned * step_phi

                            window["Degree"].update(value=str(round(curr_phi, 4)) + "[deg]")
                            curr_counter = fetch_data('script', 3).rstrip('\x00')
                        # curr_step = 0
                        curr_counter = fetch_data('script', 3).rstrip('\x00')
                        window.refresh()
                        if 'F' not in curr_counter:
                            curr_phi = int(curr_counter) * step_phi
                            window["Degree"].update(value=str(round(curr_phi, 4)) + "[deg]")
                    window.refresh()

        if event == '_Clear_': # Handle the event for clearing the list of executed files
            window['execute_file'].update(TEMP_LIST )
            window['ack_from_MSP'].update(' ')

        if event == sg.WIN_CLOSED:  # Handle the event for closing the window
            break

        if event is not None and "go_back" in event: # Handle the event for returning to the main menu
            toggle_window_layout(1, window)
        if event is not None and "go_back_script" in event:  # Handle the event for returning to the file selection screen
            toggle_window_layout(5, window)
    window.close() # Close the window when exiting the loop


if __name__ == '__main__':
    # MSP430_port = port_search()
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE, timeout=1)
    s.flush()  # clear the port
    enableTX = True
    s.set_buffer_size(1024, 1024)
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    initial_time = 1
    state = 2  # Start at neutral state
    paint_active = 0
    create_gui()
