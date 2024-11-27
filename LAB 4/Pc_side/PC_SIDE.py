import serial  # Import the pyserial library for serial communication
import time  # Import the time library for delays

def run_serial_interface():
    # Update the COM port to the correct one
    serial_conn = serial.Serial(
        port='COM3', baudrate=9600, bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=1
    )
    # Open a serial connection on COM3 with specified parameters:
    # - baudrate: 9600 bits per second
    # - bytesize: 8 bits per byte
    # - parity: no parity bit
    # - stopbits: 1 stop bit
    # - timeout: 1 second timeout for reading operations

    # Clear serial buffers
    serial_conn.reset_input_buffer()  # Clear input buffer
    serial_conn.reset_output_buffer()  # Clear output buffer

    # Define menu options
    menu_options = [
        "Please choose an action:",  # Option header
        "1- Blink RGB LED, color by color with delay of X[ms]",  # Option 1 description
        "2- Counting up onto LCD screen with delay of X[ms]",  # Option 2 description
        "3- Circular tone series via Buzzer with delay of X[ms]",  # Option 3 description
        "4- Get delay time X[ms]:",  # Option 4 description
        "5- Potentiometer 3-digit value [v] onto LCD",  # Option 5 description
        "6- Clear LCD screen",  # Option 6 description
        "7- Show menu",  # Option 7 description
        "8- Sleep"  # Option 8 description
    ]

    # Display the menu options
    for option in menu_options:
        print(option)  # Print each menu item to the console

    set_delay = False  # Flag to indicate if setting delay time is needed
    while True:  # Infinite loop for user interaction
        user_input = input('Please choose a state: ')  # User input for selected action
        if user_input == '':  # Skip empty inputs
            continue

        if user_input == '7':  # Option to display menu again
            for option in menu_options:
                print(option)  # Print each menu item to the console again
            continue  # Continue listening for input

        if user_input == '4':  # Option 4 selected, prompt for delay time in milliseconds
            set_delay = True  # Set flag to handle setting delay time

        # Convert user input to bytes and send via serial
        command_bytes = bytes(user_input + '\n', 'ascii')  # Convert string to bytes with newline character
        serial_conn.write(command_bytes)  # Send the bytes via serial
        time.sleep(0.25)  # Wait for 0.25 seconds

        # Handling setting delay time in milliseconds
        if serial_conn.out_waiting == 0 and set_delay:  # Check if serial output buffer is empty and flag is set
            delay_enabled = True  # Enable flag to set delay time
            delay_time = input("Set delay time in ms: ")  # Prompt user to set delay time in milliseconds

            # Loop until serial output buffer is empty and delay time is set
            while serial_conn.out_waiting > 0 or delay_enabled:
                if delay_time == '':  # Check if delay time is empty
                    delay_time = input("Please enter a number to set delay time in ms: ")  # Prompt again for delay time

                delay_bytes = bytes(delay_time + '\n', 'ascii')  # Convert delay time to bytes with newline character
                serial_conn.write(delay_bytes)  # Send the delay time via serial
                time.sleep(0.25)  # Wait for 0.25 seconds

                # Exit loop when serial output buffer is empty and delay time is set
                if serial_conn.out_waiting == 0:
                    delay_enabled = False  # Disable flag to exit the loop

            set_delay = False  # Reset flag after setting delay time


if __name__ == '__main__':
    run_serial_interface()  # Call the main function if the script is run directly
