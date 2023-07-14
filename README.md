# RingedDistributedHashNetwork
We are considering a scenario where the data is not located on a single machine but rather stored on  multiple machines geo-distributed across the Internet. In such a scenario, searching or retrieval of data  is very challenging, as it is hard to determine on which machine the required data is stored.

This project is related to the distributed hash table using the data structures practices 
Your project must support the following commands and options.
1. Option to specify the number of machines.
2. Option to specify the size of identifier space in bits, i.e., 160 bits, 4 bits etc.
3. Option to manually assign Id to each machine. If manual Ids are not assigned by the user, the
system must automatically assign Ids.
4. Option to insert data in the form of key, value pairs from any machine. The insertion must show the
complete path taken by the request to arrive at the correct machine. Additionally, the option to print
AVL tree.
5. Option to remove data by specifying the key from any machine. The output must show the
complete path taken by the request to arrive at the correct machine and corresponding value that is
removed from the AVL tree. The updated AVL tree must be printed.
6. Option to print the routing table of any machine.
7. Option to print the AVL tree maintain on any machine along with the location of files (and line
numbers) on which the corresponding values are stored.
8. Option to add new machines on the fly without disrupting the functionality of Ring DHT.
9. Option to remove any machine on the fly without disrupting the functionality of Ring DHT.

## Concept of Data storage and lookup in Distributed Environment
![image](https://user-images.githubusercontent.com/63167737/235335963-3818a75e-8f5a-4856-abd4-19b3c9355e36.png)
## Routing table for machines with id 1,4,9,11,14,28
![image](https://user-images.githubusercontent.com/63167737/235336059-937841bf-1fef-4262-8c72-a84828d594f0.png)
## Algorithm working

```python
function route_search_query(key, routing_table):
    # Step 1: Compute the hash of the key
    e = hash(key)
    # Step 2: Initialize p to the current machine
    p = routing_table[0]
    # Step 3: If p = e, perform AVL search on the local machine
    if p = e:
        result = avl_search(key)
        return result
    # Step 4: Find the index j such that FTp[j] < e ≤ FTp[j+1], or j = 0 if e ≤ FTp[1]
    j = 0
    for i = 0 to length(routing_table) - 2:
        if routing_table[i] < e ≤ routing_table[i+1]:
            j = i
            break
    # Step 5: If j is not found, forward the search query to the last entry in the routing table
    if j = length(routing_table) - 2 and e > routing_table[length(routing_table)-1]:
        p = routing_table[length(routing_table)-1]
    # Step 6: Otherwise, forward the search query to the machine FTp[j] and repeat the process
    else:
        p = routing_table[j]
    result = route_search_query(key, p)
    # Step 7: If the search query reaches the last machine in the routing table without finding a value for the key, return None
    return result
```

![image](https://user-images.githubusercontent.com/63167737/235336082-725e8f01-b2da-40ea-b8f9-48ae1f08663f.png)


## Commands to run 
- Open folder in terminal
- Type command
        ```
        g++ -o obj Source.cpp
        ```
