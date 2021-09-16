#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    int n = table.size();

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                if (table[i][table[j][k]] != table[table[i][j]][k])
                    return false;
    
    return true;
}
