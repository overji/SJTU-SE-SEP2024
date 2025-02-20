#include <array>
#include <iostream>
#include <vector>

bool dfs(
    int cur_x,
    int cur_y,
    int end_x,
    int end_y,
    std::vector<std::vector<int>> &matrix
) {
    static constexpr std::array<std::pair<int, int>, 4> directions {
        {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}
    };

    int n = matrix.size();
    int m = matrix[0].size();
    bool ret = false;

    if (cur_x == end_x && cur_y == end_y) {
        return true;
    }

    if(cur_x >= n || cur_y >= m || cur_x < 0 || cur_y < 0){
        return false;
    }

    if (matrix[cur_x][cur_y] == true) {
        return false;
    }

    matrix[cur_x][cur_y] = true;

    for (int i = 0; i < 4; i++) {
        ret =
            dfs(cur_x + directions[i].first,
                cur_y + directions[i].second,
                end_x,
                end_y,
                matrix);
        if (ret) {
            return true;
        }
    }

    matrix[cur_x][cur_y] = false;

    return ret;
}

int main() {
    int n, m;
    int start_x, start_y;
    int end_x, end_y;

    std::cin >> n >> m;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    std::cin >> start_x >> start_y;
    std::cin >> end_x >> end_y;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> matrix[i][j];
        }
    }

    std::cout << dfs(start_x, start_y, end_x, end_y, matrix) << std::endl;
}
