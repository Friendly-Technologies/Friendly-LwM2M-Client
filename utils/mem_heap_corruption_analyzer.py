#!/usr/bin/env python3

# For analyze heap memory usage and detect leak or double free, add next lines to
# main.cpp, after that collect logs to the file and run this script with retrieved logs.
#
# void* operator new(std::size_t sz) {
#     heap_usage += sz;
# 	void *ptr = std::malloc(sz);
# 	cout << "[_NEW_] [" << hex << ptr << dec << "] [" << sz << "]" << endl;
#     return ptr;
# }
#
# void* operator new[](std::size_t sz) {
#     heap_usage += sz;
#     void *ptr = std::malloc(sz);
# 	cout << "[_NEW_] [" << hex << ptr << dec << "] [" << sz << "]" << dec << endl;
#     return ptr;
# }
#
# void operator delete(void* ptr) noexcept {
# 	cout << "[_DELETE_] [" << hex << ptr << "]" << dec << endl;
#     std::free(ptr);
# }
#
# void operator delete[](void* ptr) noexcept {
# 	cout << "[_DELETE_] [" << hex << ptr << "]" <<  dec << endl;
#     std::free(ptr);
# }

import sys

NEW_ACT = "_NEW_"
DELETE_ACT = "_DELETE_"
FIELD_START = '['
FIELD_END = ']'
MEM_INFO_PREFIX = "mem_heap_analyze_logs.txt"

ACT_TYPE_ID = 0
ACT_ADDR_ID = 1
ACT_SIZE_ID = 2
ACT_LOGS_POS_ID = 3


def parse_line(line: str):
    action = dict()
    if NEW_ACT not in line and DELETE_ACT not in line:
        return action

    type_start = line.find(FIELD_START)
    type_end = line.find(FIELD_END, type_start)
    if type_start == -1 or type_end == -1:
        print("ERR: incorrect line format [type] -> ", line)
        return dict()
    # Set action type
    action[ACT_TYPE_ID] = line[type_start+1:type_end]

    # Determine address position
    addr_start = line.find(FIELD_START, type_end)
    addr_end = line.find(FIELD_END, addr_start)
    if addr_start == -1 or addr_end == -1:
        print("ERR: incorrect line format [addr] -> ", line)
        return dict()
    # Set action address
    action[ACT_ADDR_ID] = line[addr_start+1:addr_end]

    # Determine size position
    if action[ACT_TYPE_ID] == NEW_ACT:
        size_start = line.find(FIELD_START, addr_end)
        size_end = line.find(FIELD_END, size_start)
        if size_start == -1 or size_end == -1:
            print("ERR: incorrect line format [size] -> ", line)
            return dict()
        # Set action size
        action[ACT_SIZE_ID] = line[size_start+1:size_end]
    return action


def parse_file(log_file_name:str):
    data = list()
    with open(log_file_name) as f:
        line_cnt =1
        for line in f:
            action = parse_line(line)
            if action:
                action[ACT_LOGS_POS_ID] = str(line_cnt)
                data.append(action)
            line_cnt += 1
    return data


def burn_opposite_same_act(actions):
    acts_for_rm = list()
    for new_id in range(len(actions)):
        if actions[new_id][ACT_TYPE_ID] != NEW_ACT:
            continue
        for del_id in range(len(actions)):
            if actions[del_id][ACT_TYPE_ID] != DELETE_ACT or del_id in acts_for_rm:
                continue
            if actions[new_id][ACT_ADDR_ID] == actions[del_id][ACT_ADDR_ID]:
                acts_for_rm.append(new_id)
                acts_for_rm.append(del_id)
                break
    print(f"\n----- COMMON ACTIONS {len(actions)}, BURNED {len(acts_for_rm)} -----")
    acts_for_rm.sort(reverse=True)
    for act_id in acts_for_rm:
        del actions[act_id]


def analyze_corrupted_mem_actions(actions):
    double_free_mem = list()
    double_free_mem_cnt = 0
    mem_leak = list()
    mem_leak_size = 0
    for action in actions:
        if action[ACT_TYPE_ID] == NEW_ACT:
            mem_leak.append(action)
            mem_leak_size += int(action[ACT_SIZE_ID], 10)
        elif action[ACT_TYPE_ID] == DELETE_ACT:
            double_free_mem.append(action)
            double_free_mem_cnt += 1

    print(f"\n----- MEMORY LEAK SIZE {mem_leak_size} BYTES {len(mem_leak)} CNT -----")
    for leak in mem_leak:
        print(leak)

    print(f"\n----- DOUBLE FREE MEM CNT {double_free_mem_cnt} -----")
    for double_free in double_free_mem:
        print(double_free)


def find_mem_action_by_line_id(actions, line_id):
    for action in actions:
        if int(action[ACT_LOGS_POS_ID]) == line_id:
            return action
    return dict()


def add_corrupted_mem_info_to_logs(actions, log_file_name, new_file_with_analyze):
    with open(new_file_with_analyze, 'w') as new_file:
        new_file.truncate(0)
        with open(log_file_name) as old_file:
            log_lines = old_file.readlines()
            for line_id in range(len(log_lines)):
                action = find_mem_action_by_line_id(actions, line_id+1)
                if action:
                    if action[ACT_TYPE_ID] == NEW_ACT:
                        new_file.write("!!!!! [MEMORY CORRUPTED] LEAK !!!!! " + log_lines[line_id])
                    elif action[ACT_TYPE_ID] == DELETE_ACT:
                        new_file.write("!!!!! [MEMORY CORRUPTED] DOUBLE FREE !!!!! " + log_lines[line_id])
                else:
                    new_file.write(log_lines[line_id])
    print(f"\n----- CREATED {new_file_with_analyze} -----")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("No argument provided.\n"
              "Pleas set path to file with logs.\n"
              "Ex: python3 mem_heap_corruption_analyzer logs.txt")
        sys.exit()
    
    log_file_name = sys.argv[1]
    
    mem_actions = parse_file(log_file_name)
    burn_opposite_same_act(mem_actions)
    analyze_corrupted_mem_actions(mem_actions)
    add_corrupted_mem_info_to_logs(mem_actions, log_file_name, MEM_INFO_PREFIX)
