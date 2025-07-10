import subprocess
import sys
from pathlib import Path

class TestChecker:
    def __init__(self, test_dir, program_path):
        self.program_path = program_path
        self.test_dir = test_dir
        self.test_cases = [
            ("test1_basic_operations.txt", "output1_expected.txt"),
            ("test2_cyclic_graph.txt", "output2_expected.txt"),
            ("test3_disconnected.txt", "output3_expected.txt"),
            ("test4_error_handling.txt", "output4_expected.txt"),
            ("test5_dijkstra.txt", "output5_expected.txt"),
            ("test6_tarjan.txt", "output6_expected.txt")
        ]

        self.test_cases = [
            (f"{test_dir}{test}", f"{test_dir}{output}") 
            for test, output in self.test_cases
        ]
    
    def run_program(self, input_file):
        with open(input_file, 'r') as f:
            input_data = f.read()
        
        result = subprocess.run(
            [self.program_path],
            input=input_data.encode('utf-8'),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        
        return result.stdout.decode('utf-8').strip()
    
    def compare_outputs(self, actual, input_file, expected_file):
        with open(expected_file, 'r') as f:
            expected = f.read().strip()
        
        actual_lines = [line.strip() for line in actual.split('\n') if line.strip()]
        expected_lines = [line.strip() for line in expected.split('\n') if line.strip()]
        
        ok_ouput = {}
        for element in expected_lines:
            ok_ouput[''.join(sorted(element))] = ok_ouput.get(''.join(sorted(element)), 0) + 1
            # ok_ouput[element] = ok_ouput.get(element, 0) + 1
        
        if len(actual_lines) != len(expected_lines):
            return False, f"Number of lines does not correspond: expected {len(expected_lines)}, recieved {len(actual_lines)}"
        
        for actual_line in actual_lines:
            line = ''.join(sorted(actual_line))
            # line = actual_line
            if line in ok_ouput:
                ok_ouput[line] -= 1
        
        if all(cnt == 0 for cnt in ok_ouput.values()):
            return True, "All tests passed"
        else:
            return False, "Test failed"
    
    def run_all_tests(self):
        total_tests = len(self.test_cases)
        passed_tests = 0
        
        for input_file, expected_file in self.test_cases:
            if not Path(input_file).exists():
                print(f"Input file {input_file} isn't present, skip test")
                continue
            if not Path(expected_file).exists():
                print(f"Output file {expected_file} isn't present, skip test")
                continue
            
            print(f"\nExecuting {input_file}...")
            
            try:
                actual_output = self.run_program(input_file)
                success, message = self.compare_outputs(actual_output, input_file, expected_file)
                
                if success:
                    print(f"✅ Test {input_file} passed")
                    passed_tests += 1
                else:
                    print(f"❌ Test {input_file} didn't pass")
                    print(message)
            except Exception as e:
                print(f"⚠️ Error while executing test {input_file}: {str(e)}")
        
        print(f"\nSummary: {passed_tests}/{total_tests} tests passed")
        return passed_tests == total_tests

def main():
    # if len(sys.argv) != 2:
    #     print("Usage: python checker.py <path_to_executable>")
    #     sys.exit(1)
    
    checker = TestChecker("tests/", "build/graphs")
    checker.run_all_tests()

if __name__ == "__main__":
    main()