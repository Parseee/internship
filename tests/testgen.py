import random
import sys
from itertools import combinations

def generate_random_label(length=2):
    """Генерация случайной метки узла"""
    return ''.join(random.choices('ABCDEFGHIJKLMNOPQRSTUVWXYZ', k=length))

def generate_test_case_1(num_nodes=10, num_edges=15):
    """Базовый тест: создание узлов и ребер"""
    nodes = [generate_random_label() for _ in range(num_nodes)]
    edges = []
    
    # Убедимся, что нет дубликатов узлов
    nodes = list(set(nodes))
    
    # Создаем ребра между случайными узлами
    for _ in range(num_edges):
        if len(nodes) < 2:
            break
        a, b = random.sample(nodes, 2)
        weight = random.randint(1, 100)
        edges.append((a, b, weight))
    
    # Генерируем команды
    commands = []
    for node in nodes:
        commands.append(f"NODE {node}")
    
    for a, b, weight in edges:
        commands.append(f"EDGE {a} {b} {weight}")
    
    # Добавим несколько операций удаления
    if len(nodes) > 3:
        commands.append(f"REMOVE NODE {nodes[0]}")
        commands.append(f"REMOVE EDGE {edges[0][0]} {edges[0][1]}")
    
    # Добавим RPO нумерацию
    if len(nodes) > 1:
        commands.append(f"RPO_NUMBERING {random.choice(nodes[1:])}")
    
    return commands

def generate_test_case_2():
    """Тест с циклическим графом"""
    nodes = ['A', 'B', 'C', 'D']
    edges = [
        ('A', 'B', 5),
        ('B', 'C', 3),
        ('C', 'D', 2),
        ('D', 'A', 4)
    ]
    
    commands = []
    for node in nodes:
        commands.append(f"NODE {node}")
    
    for a, b, weight in edges:
        commands.append(f"EDGE {a} {b} {weight}")
    
    commands.append("RPO_NUMBERING A")
    commands.append("TARJAN A")
    commands.append("DUJKSTRA A")
    commands.append("MAX FLOW A C")
    
    return commands

def generate_test_case_3():
    """Тест с несвязными компонентами"""
    nodes = ['X', 'Y', 'Z', 'W']
    
    commands = []
    for node in nodes:
        commands.append(f"NODE {node}")
    
    # Добавим только одно ребро
    commands.append("EDGE X Y 10")
    
    commands.append("RPO_NUMBERING X")
    commands.append("TARJAN X")
    commands.append("DUJKSTRA X")
    commands.append("MAX FLOW X W")  # Должен вернуть 0, так как пути нет
    
    return commands

def generate_test_case_4():
    """Тест с ошибками (попытка удалить несуществующие узлы/ребра)"""
    nodes = ['P', 'Q', 'R']
    edges = [('P', 'Q', 7)]
    
    commands = []
    for node in nodes:
        commands.append(f"NODE {node}")
    
    for a, b, weight in edges:
        commands.append(f"EDGE {a} {b} {weight}")
    
    # Добавляем команды, которые должны вызвать ошибки
    commands.append("REMOVE NODE NON_EXISTENT")
    commands.append("REMOVE EDGE P NON_EXISTENT")
    commands.append("REMOVE EDGE NON_EXISTENT_A NON_EXISTENT_B")
    commands.append("EDGE P NON_EXISTENT 5")
    commands.append("EDGE NON_EXISTENT_A NON_EXISTENT_B 5")
    
    return commands

def generate_test_case_5():
    """Тест для алгоритма Дейкстры"""
    nodes = ['S', 'A', 'B', 'C', 'D', 'E', 'F', 'T']
    edges = [
        ('S', 'A', 4), ('S', 'B', 3),
        ('A', 'C', 2), ('B', 'A', 1),
        ('B', 'C', 4), ('B', 'D', 5),
        ('C', 'D', 2), ('C', 'E', 1),
        ('D', 'T', 3), ('E', 'T', 2),
        ('E', 'F', 2), ('F', 'T', 2)
    ]
    
    commands = []
    for node in nodes:
        commands.append(f"NODE {node}")
    
    for a, b, weight in edges:
        commands.append(f"EDGE {a} {b} {weight}")
    
    commands.append("DUJKSTRA S")
    commands.append("MAX FLOW S T")
    
    return commands

def generate_test_case_6():
    """Тест для алгоритма Тарьяна"""
    nodes = ['1', '2', '3', '4', '5', '6', '7', '8']
    edges = [
        ('1', '2', 1), ('2', '3', 1), ('3', '1', 1),
        ('4', '2', 1), ('4', '3', 1), ('4', '5', 1),
        ('5', '4', 1), ('5', '6', 1),
        ('6', '7', 1), ('7', '6', 1),
        ('8', '7', 1), ('8', '5', 1), ('8', '8', 1)
    ]
    
    commands = []
    for node in nodes:
        commands.append(f"NODE {node}")
    
    for a, b, weight in edges:
        commands.append(f"EDGE {a} {b} {weight}")
    
    commands.append("TARJAN 1")
    commands.append("RPO_NUMBERING 1")
    
    return commands

def save_test_case(commands, filename):
    """Сохранить тестовый случай в файл"""
    with open(filename, 'w') as f:
        for cmd in commands:
            f.write(cmd + '\n')

def main():
    """Главная функция генерации тестов"""
    test_cases = [
        ("test1_basic_operations.txt", generate_test_case_1()),
        ("test2_cyclic_graph.txt", generate_test_case_2()),
        ("test3_disconnected.txt", generate_test_case_3()),
        ("test4_error_handling.txt", generate_test_case_4()),
        ("test5_dijkstra.txt", generate_test_case_5()),
        ("test6_tarjan.txt", generate_test_case_6())
    ]
    
    for filename, commands in test_cases:
        save_test_case(commands, filename)
        print(f"Сгенерирован тестовый файл: {filename}")

if __name__ == "__main__":
    main()