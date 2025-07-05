import subprocess
import os
import re
import time

# --- Configuration ---
MINISHELL_PATH = "./minishell"
SUPPRESSION_FILE = "./readline.supp"  # Suppression file for readline leaks
TIMEOUT = 10  # Valgrind is slower, so we need a longer timeout

# Colors for printing
class bcolors:
    OKGREEN = '\033[92m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    BLUE = '\033[94m'
    WARNING = '\033[93m'

# --- Test Cases ---
# Same set of commands to test different code paths
test_cases = [
    "ls -l",
    "echo hello",
    "cat /dev/null",
    "< Makefile cat | grep 'NAME' | wc -l > out.txt",
    "echo hello > file1",
    "cat < file1",
    "cat << EOF\nhello\nworld\n$USER\nEOF",
    "cat << 'EOF'\nno expand $USER\nEOF",
    "export VAR=test; echo $VAR",
    "ls | cat | wc",
    "invalid_command_should_not_leak",
    "< non_existent_file",
    "echo \"$PWD\" and '$HOME'",
    "", # Test empty line
]

def cleanup_files():
    """Removes files created during testing."""
    files_to_remove = ["out.txt", "file1"]
    for f in files_to_remove:
        if os.path.exists(f):
            os.remove(f)

def run_valgrind_for_command(command):
    """
    Runs minishell through Valgrind for a single test case.
    Returns the full stderr output from Valgrind.
    """
    try:
        valgrind_cmd = [
            "valgrind",
            "--leak-check=full",
            "--show-leak-kinds=all",
            f"--suppressions={SUPPRESSION_FILE}",
            MINISHELL_PATH
        ]
        
        minishell_input = f"{command}\nexit\n"
        
        proc = subprocess.Popen(valgrind_cmd, 
                              stdin=subprocess.PIPE, 
                              stdout=subprocess.PIPE, 
                              stderr=subprocess.PIPE, 
                              text=True)

        _stdout, stderr = proc.communicate(input=minishell_input, timeout=TIMEOUT)
        return stderr

    except FileNotFoundError:
        return "Tester Error: 'valgrind' command not found. Please install it."
    except subprocess.TimeoutExpired:
        proc.kill()
        return "Tester Error: TIMEOUT - Valgrind took too long to execute."
    except Exception as e:
        return f"Tester Error: An unexpected error occurred: {e}"

def main():
    if not os.path.exists(MINISHELL_PATH):
        print(f"{bcolors.FAIL}Error: Minishell executable not found at '{MINISHELL_PATH}'{bcolors.ENDC}")
        return
    if not os.path.exists(SUPPRESSION_FILE):
        print(f"{bcolors.FAIL}Error: Suppression file not found at '{SUPPRESSION_FILE}'{bcolors.ENDC}")
        print(f"{bcolors.WARNING}Please create it before running the test.{bcolors.ENDC}")
        return

    print(f"{bcolors.BOLD}--- Minishell Valgrind Leak Test Report ---{bcolors.ENDC}")
    leaked_count = 0
    clean_count = 0

    for i, command in enumerate(test_cases):
        cleanup_files()

        print(f"\n{bcolors.BOLD}--- Test #{i+1} ---{bcolors.ENDC}")
        print(f"  {bcolors.BLUE}COMMAND: {repr(command)}{bcolors.ENDC}")

        valgrind_output = run_valgrind_for_command(command)
        
        # Valgrind's success condition is "0 bytes in 0 blocks" for definite and indirect losses
        leak_found = "definitely lost: 0 bytes in 0 blocks" not in valgrind_output or \
                     "indirectly lost: 0 bytes in 0 blocks" not in valgrind_output
        
        if not leak_found:
            clean_count += 1
            print(f"  {bcolors.OKGREEN}✓ CLEAN - No leaks found.{bcolors.ENDC}")
        else:
            leaked_count += 1
            print(f"  {bcolors.FAIL}✗ LEAKS DETECTED{bcolors.ENDC}")
            print(bcolors.WARNING + "-" * 10 + " VALGRIND REPORT " + "-" * 10 + bcolors.ENDC)
            # Find the summary section and print it
            summary_match = re.search(r'==\d+==\s+LEAK SUMMARY:.*==\d+==\s+ERROR SUMMARY:', valgrind_output, re.DOTALL)
            if summary_match:
                 print(summary_match.group(0).strip())
            else: # If summary isn't found, print the whole thing
                 print(valgrind_output)
            print(bcolors.WARNING + "-" * 37 + bcolors.ENDC)

    cleanup_files()

    print("\n" + "="*40)
    print(f"{bcolors.BOLD}--- LEAK SUMMARY ---{bcolors.ENDC}")
    print(f"  {bcolors.OKGREEN}✓ {clean_count} Commands were leak-free.{bcolors.ENDC}")
    print(f"  {bcolors.FAIL}✗ {leaked_count} Commands had leaks.{bcolors.ENDC}")
    print("="*40)

if __name__ == "__main__":
    main()