cmake_minimum_required(VERSION 3.27)
project(MySTL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(MySTL
        main.cpp
        src/String.cpp
        src/Allocator.cpp
        include/List.h
        include/Deque.h
        include/Queue.h
        include/ForwardList.h
        include/PriorityQueue.h
        include/Pair.h
        include/HashMap.h
        include/HashSet.h
        include/Map.h
        include/Set.h
        include/Vector.h
        include/HashMultiMap.h
        include/HashMultiSet.h
        include/MultiMap.h
        include/utils/RBTreeNode.h
        include/MultiSet.h
        include/ReverseIterator.h
        include/Stack.h
        include/Functional.h
        include/Memory/UniquePtr.h
        include/Memory/SharedPtr.h
        include/Memory/WeakPtr.h
        include/Memory/Allocator.h
        include/Log/Logger.h
        include/Log/ConsoleLogger.h
        include/Log/FileLogger.h
        include/Concurrent/ConcurrentVector.h
        include/Concurrent/ConcurrentQueue.h
        include/Concurrent/ConCurrentStack.h
)

# target_link_libraries(MySTL PRIVATE SomeOtherLibrary)
