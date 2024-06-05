#!/usr/bin/env python3

# @file HeapMemoryAnalyzer.py
# @version 1.0.0
# @ref https://github.com/VSkoshchuk/HeapMemoryAnalyzer
#
# MIT License
# Copyright (c) 2023 VSkoshchuk skoschuk999@gmail.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# USAGE:
# For analyze heap memory usage and detect leak or double free, add next lines to
# main.cpp, after that collect logs to the file and run this script with retrieved logs.
#
# void* operator new(std::size_t sz) {
#   void *ptr = std::malloc(sz);
#   cout << endl << "[_NEW_:" << hex << ptr << dec << ":" << sz << "]" << endl;
#   return ptr;
# }
#
# void* operator new[](std::size_t sz) {
#   void *ptr = std::malloc(sz);
#   cout << endl << "[_NEW_:" << hex << ptr << dec << ":" << sz << "]" << dec << endl;
#   return ptr;
# }
#
# void operator delete(void* ptr) noexcept {
#   cout << endl << "[_DELETE_:" << hex << ptr << "]" << dec << endl;
#   std::free(ptr);
# }
#
# void operator delete[](void* ptr) noexcept {
#   cout << endl << "[_DELETE_:" << hex << ptr << "]" <<  dec << endl;
#   std::free(ptr);
# }

import sys
from typing import Optional
from typing import Tuple


DEFAULT_OUT_FILE_PATH = "mem_heap_analyze_info_with_logs.txt"


class MemAct:
    NEW_ACT = "_NEW_"
    DELETE_ACT = "_DELETE_"

    def __init__(self, type:str, addr:str, mem_size:str, id:int):
        if type not in [MemAct.NEW_ACT, MemAct.DELETE_ACT]:
            raise Exception(f"Incorrect memory action type {type}")
        self.__type = type
        try:
            int(addr, 16)
        except Exception as e:
            raise Exception(f"Incorrect memory address {addr}")
        self.__addr = addr
        self.__id = id
        try:
            self.__mem_size = int(mem_size, 10)
        except Exception as e:
            raise Exception(f"Incorrect memory size {mem_size}")

    def is_new(self) -> bool:
        return self.get_type() == MemAct.NEW_ACT

    def is_delete(self) -> bool:
        return self.get_type() == MemAct.DELETE_ACT

    def get_type(self) -> str:
        return self.__type

    def get_addr(self) -> str:
        return self.__addr

    def get_id(self) -> int:
        return self.__id

    def get_mem_size(self) -> int:
        return self.__mem_size

    def to_str(self):
        return f"TYPE->{self.get_type()}, ADDR->{self.get_addr()}, MEM SIZE->{self.get_mem_size()}, ID->{self.get_id()}"


class CorruptedMemInfo:
    def __init__(self, corrupted_acts:list[MemAct]):
        self.__leak_acts = list()
        self.__leak_size = 0
        self.__double_free_acts = list()
        self.__corrupted_acts = corrupted_acts

        for action in corrupted_acts:
            if action.is_new():
                self.__leak_acts.append(action)
                self.__leak_size += action.get_mem_size()
            elif action.is_delete():
                self.__double_free_acts.append(action)

    def get_leak_acts(self) -> list[MemAct]:
        return self.__leak_acts

    def get_mem_leak_size(self) -> int:
        return self.__leak_size

    def get_double_free_acts(self) -> list[MemAct]:
        return self.__double_free_acts

    def get_corrupted_acts(self):
        return self.__corrupted_acts


class MemActsLogParser:
    FIELD_START = '['
    FIELD_END = ']'
    FIELD_SEPARATOR = ":"

    NEW_FIELDS_CNT = 3
    DELETE_FIELDS_CNT = 2

    TYPE_POS = 0
    ADDR_POS = 1
    MEM_SIZE_POS = 2

    def __init__(self, logs_file_path:str):
        print(f"MemActsLogParser: Parsing file '{logs_file_path}'")
        self.__logs_file = logs_file_path
        print(f"MemActsLogParser: Extracting each memory action from logs")
        self.__mem_acts = self.__get_mem_acts()
        print(f"MemActsLogParser: Computing corrupted memory actions")
        self.__corrupted_acts = self.__get_corrupted_mem_acts()
        print(f"MemActsLogParser: Computing max heap usage and position in logs")
        self.__max_heap_size, self.__max_heap_pos = self.__get_max_heap_usage()

    def get_mem_acts(self) -> list[MemAct]:
        return self.__mem_acts

    def get_corrupted_acts_info(self) -> CorruptedMemInfo:
        return self.__corrupted_acts

    def get_max_heap_usage_size(self) -> int:
        return self.__max_heap_size

    def get_max_heap_usage_pos(self) -> int:
        return self.__max_heap_pos

    def create_report(self, out_file_path:str = DEFAULT_OUT_FILE_PATH):
        print(f"MemActsLogParser: Creating report in the file '{out_file_path}'")
        with open(out_file_path, 'w') as new_file:
            new_file.truncate(0)
            corrupted_info = self.get_corrupted_acts_info()

            new_file.write(f"################ COMMON MEMORY ACTIONS {len(self.get_mem_acts())}, CORRUPTED {len(corrupted_info.get_corrupted_acts())}\n")
            new_file.write( f"\n################ MEMORY MAX HEAP USAGE {self.get_max_heap_usage_size()} BYTES, AT LINE  {self.get_max_heap_usage_pos()}\n")
            new_file.write(f"\n################ MEMORY LEAK SIZE {corrupted_info.get_mem_leak_size()} BYTES, ACTION {len(corrupted_info.get_leak_acts())} COUNT\n")
            for leak in corrupted_info.get_leak_acts():
                new_file.write(f"######## {leak.to_str()}\n")
            new_file.write(f"\n################ DOUBLE FREE MEMORY ACTIONS COUNT {len(corrupted_info.get_double_free_acts())}\n")
            for double_free in corrupted_info.get_double_free_acts():
                new_file.write(f"######## {double_free.to_str()}\n")

            new_file.write(f"\n################ LOGS WITH INFORMATION ABOUT MEMORY CORRUPTION\n")
            with open(self.__logs_file, 'r', encoding='utf-8', errors='ignore') as old_file:
                log_lines = old_file.readlines()
                for line_id in range(len(log_lines)):
                    line = log_lines[line_id]
                    if line_id+1 == self.get_max_heap_usage_pos():
                        line = f"######## [MAX HEAP USAGE] [{self.get_max_heap_usage_size()}]   " + line
                    action = self.__find_mem_action_by_line_id(corrupted_info.get_corrupted_acts(), line_id+1)
                    if action is not None:
                        if action.is_new():
                            new_file.write("######## [MEMORY CORRUPTED] [LEAK]   " + line)
                        elif action.is_delete():
                            new_file.write("######## [MEMORY CORRUPTED] [DOUBLE FREE]  " + line)
                    else:
                        new_file.write(line)
            new_file.write(f"\n\n\n")

    def __get_mem_acts(self) -> list[MemAct]:
        data = list()
        with open(self.__logs_file, 'r', encoding='utf-8', errors='ignore') as f:
            line_cnt = 1
            for line in f:
                action = self.__parse_log_line(line, line_cnt)
                if action is not None:
                    data.append(action)
                line_cnt += 1
        return data

    def __get_act_block_pos(self, log_line:str) -> Optional[Tuple[int, int]]:
        start = -1
        if MemAct.NEW_ACT in log_line:
            start = log_line.find(self.FIELD_START + MemAct.NEW_ACT)
            if start == -1:
                print(f"Incorrect log line format start: {log_line}")
                return None
        elif MemAct.DELETE_ACT in log_line:
            start = log_line.find(self.FIELD_START + MemAct.DELETE_ACT)
            if start == -1:
                print(f"Incorrect log line format start: {log_line}")
                return None
        else:
            return None

        end = log_line.find(self.FIELD_END, start)
        if end == -1:
            print(f"Incorrect log line format end: {log_line}")
            return None

        return (start, end)

    def __parse_log_line(self, log_line: str, log_line_pos: int) -> Optional[MemAct]:
        # Line format: [_NEW_:addr:size]
        #              [_DELETE_:addr]
        pos = self.__get_act_block_pos(log_line)
        if pos is None:
            return None

        fields = log_line[pos[0]+1:pos[1]].split(self.FIELD_SEPARATOR)
        if MemAct.NEW_ACT in log_line and len(fields) != self.NEW_FIELDS_CNT:
            print(f"Incorrect NEW_ACT fields cnt: {log_line}")
            return None
        elif MemAct.DELETE_ACT in log_line and len(fields) != self.DELETE_FIELDS_CNT:
            print(f"Incorrect DELETE_ACT fields cnt: {log_line}")
            return None

        act_type = fields[self.TYPE_POS]
        act_addr = fields[self.ADDR_POS]
        act_mem_size = "0"
        if act_type == MemAct.NEW_ACT:
            act_mem_size = fields[self.MEM_SIZE_POS]

        try:
            return MemAct(act_type, act_addr, act_mem_size, log_line_pos)
        except Exception as e:
            print(e)
            return None

    def __get_corrupted_mem_acts(self) -> CorruptedMemInfo:
        paired_acts = list()
        for new_id in range(len(self.__mem_acts)):
            if self.__mem_acts[new_id].is_delete():
                continue
            for del_id in range(new_id, len(self.__mem_acts)):
                if self.__mem_acts[del_id].is_new() or del_id in paired_acts:
                    continue
                if self.__mem_acts[new_id].get_addr() == self.__mem_acts[del_id].get_addr():
                    paired_acts.append(new_id)
                    paired_acts.append(del_id)
                    break

        corrupted = list()
        for act_id in range(len(self.__mem_acts)):
            if act_id not in paired_acts:
                corrupted.append(self.__mem_acts[act_id])
        return CorruptedMemInfo(corrupted)

    # returns max value, position in logs
    def __get_max_heap_usage(self) -> Tuple[int, int]:
        max_usage_size = 0
        max_usage_pos = -1
        common_usage = 0
        used_ids = list()

        for id in range(len(self.__mem_acts)):
            act = self.__mem_acts[id]
            if act.is_new():
                common_usage += act.get_mem_size()
                if max_usage_size < common_usage:
                    max_usage_size = common_usage
                    max_usage_pos = act.get_id()
            elif act.is_delete():
                for rid in range(id-1, -1, -1):
                    ract = self.__mem_acts[rid]
                    if ract.is_new() and ract.get_addr() == act.get_addr():
                        if act.get_id() not in used_ids:
                            used_ids.append(act.get_id())
                            common_usage -= ract.get_mem_size()
                        else:
                            break
        return max_usage_size, max_usage_pos

    def __find_mem_action_by_line_id(self, actions:list[MemAct], line_id) -> Optional[MemAct]:
        for action in actions:
            if action.get_id() == line_id:
                return action
        return None


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("No argument provided.\n"
              "Pleas set path to file with logs.\n"
              "Ex: python3 mem_heap_corruption_analyzer logs.txt [OPTIONAL: out file path]")
        sys.exit()
    
    logs_file_name = str(sys.argv[1])
    out_file_name = DEFAULT_OUT_FILE_PATH
    if len(sys.argv) == 3:
        out_file_name = sys.argv[2]

    parser = MemActsLogParser(logs_file_name)
    parser.create_report(out_file_name)

    print(f"Created file with report -> {out_file_name}")