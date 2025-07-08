import subprocess
import os
import re

# --- Configuration ---
MINISHELL_PATH = "./minishell"
BASH_PATH = "/bin/bash"
TIMEOUT = 3  # seconds to wait for each command

# Colors for printing
class bcolors:
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    BLUE = '\033[94m'

# --- Test Cases ---
test_cases = [
    # Exit Status ($?) Expansion Tests
    '"$?"', '"$?"$', '"$?"$$', '"$?"$$$$', '$"$?"', '$$"$?"', '$$$"$?"', '$$$$"$?"', '$$$$?"$?"',
    "$$$'$?'\"$?\"", "'$?'\"$?\"", "'$?'\"$?\"", "'$?'\"$?\"$?", "'$?'\"$?\" $?",
    "echo $'$?'\"$?\"$?", "echo $'$?'\"$?\" $?", "echo $'$?'\"$?\"     '$?'", "echo $'$?'\"$?\"     \"$?\"",

    # Variable and Quote Expansion Tests
    'echo \'"$GOME"\'', 'echo \'"$HOME"\'', 'echo "$HOME"', "echo \"'$HOME'\"", "echo \"'$HOME'\"\"\"",
    "echo \"'$HOME''\"", "echo \"$PWD'$HOME'\"", "echo \"\"$PWD\"'$HOME'\"", "echo $PWD\"'$HOME'\"",
    "echo $PWD'$HOME'", "echo $PWD '$HOME'", "echo $PWD' $HOME'", "echo $PWD  '$HOME'",
    "echo $PWD  \"'$HOME'\"", "echo $PWD  '$HOME'$HOME", "echo $PWD'$HOME'$HOME",

    # Export and field splitting tests
    'export mahdi="\'ls"; echo $mahdi', 'export mahdi="\'ls"; echo "$mahdi"',
    'export mahdi="\'ls"; echo $mahdi$mahdi', 'export mahdi=""; echo $mahd',
    "export mahdi=\"' ls\"; echo '$mahdi'$mahdi",

    # More complex combinations
    "echo '$HOME'$HOME", "echo $HOME'$HOME'", "echo \"$HOME\"'$HOME'", "echo \"$HOME\" '$HOME'",
    "echo $HOME  '$HOME'", "echo \"$HOME'$HOME'\"", "echo '\"$HOME'$HOME'\"'",
    "echo '\"$HOME$HOME\"'", "echo \"'\"$HOME$HOME\"'\"", "\"'\"$HOME", "\"'\"$HOME$HOME",
    "echo \"'\"$HOME$HOME", "echo \"\"$HOME$HOME", "echo $HOME$HOME", "echo $HOME$HOME$HOME",
    "echo $HOME$HOME$HOME$HOME",
]

def run_command(shell_path, command):
    """Runs a command in a given shell and returns its output."""
    try:
        env = os.environ.copy()
        if "bash" in shell_path:
            proc = subprocess.run([shell_path, "-c", command],
                                  capture_output=True, text=True, timeout=TIMEOUT, env=env)
        else:
            command_with_exit = command + '\nexit\n'
            proc = subprocess.run([shell_path], input=command_with_exit,
                                  capture_output=True, text=True, timeout=TIMEOUT, env=env)
        return proc.stdout.strip(), proc.stderr.strip()
    except Exception as e:
        return "", f"Tester Error: {e}"

def main():
    if not os.path.exists(MINISHELL_PATH):
        print(f"{bcolors.FAIL}Error: Minishell executable not found at '{MINISHELL_PATH}'{bcolors.ENDC}")
        return

    print(f"{bcolors.BOLD}--- Minishell Expansion Test Report ---{bcolors.ENDC}")
    success_count = 0
    valid_diff_count = 0
    fail_count = 0
    
    # Set a consistent exit code for $? tests
    run_command(BASH_PATH, "ls non_existent_file_123 >/dev/null 2>&1")
    run_command(MINISHELL_PATH, "ls non_existent_file_123 >/dev/null 2>&1")

    for i, command in enumerate(test_cases):
        m_out, m_err = run_command(MINISHELL_PATH, command)
        b_out, b_err = run_command(BASH_PATH, command)

        m_full_output = (m_out + " " + m_err).strip().replace("minishell> ", "").strip()
        b_full_output = (b_out + " " + b_err).strip()

        is_bash_error = "command not found" in b_err or "No such file or directory" in b_err
        is_minishell_error = "command not found" in m_err or "No such file or directory" in m_err
        
        # --- Categorize and Print Result ---
        print(f"\n{bcolors.BOLD}--- Test #{i+1} ---{bcolors.ENDC}")
        print(f"  {bcolors.BLUE}COMMAND: {command}{bcolors.ENDC}")

        if m_full_output == b_full_output:
            success_count += 1
            print(f"  {bcolors.OKGREEN}✓ SUCCESS (Exact Match){bcolors.ENDC}")
            print(f"    {bcolors.OKGREEN}Output: {m_full_output}{bcolors.ENDC}")
        elif is_bash_error and is_minishell_error:
            valid_diff_count += 1
            print(f"  {bcolors.WARNING}~ VALID DIFFERENCE (Both Error){bcolors.ENDC}")
            print(f"    {bcolors.WARNING}BASH: {b_full_output}{bcolors.ENDC}")
            print(f"    {bcolors.WARNING}MINISHELL: {m_full_output}{bcolors.ENDC}")
        else:
            fail_count += 1
            print(f"  {bcolors.FAIL}✗ FAILURE (Output Mismatch){bcolors.ENDC}")
            print(f"    {bcolors.WARNING}BASH Expected: {b_full_output}{bcolors.ENDC}")
            print(f"    {bcolors.FAIL}MINISHELL Got: {m_full_output}{bcolors.ENDC}")
    
    # --- FINAL SUMMARY ---
    print("\n" + "="*40)
    print(f"{bcolors.BOLD}--- TEST SUMMARY ---{bcolors.ENDC}")
    print(f"  {bcolors.OKGREEN}✓ {success_count} Passed (Exact Match){bcolors.ENDC}")
    print(f"  {bcolors.WARNING}~ {valid_diff_count} Passed (Valid Difference){bcolors.ENDC}")
    print(f"  {bcolors.FAIL}✗ {fail_count} Failed{bcolors.ENDC}")
    print("="*40)


if __name__ == "__main__":
    main()