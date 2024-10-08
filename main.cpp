#include <iostream>
#include <sstream>
#include "Cell.h"
#include "Grid.h"
#include "Solver.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <chrono>

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
Grid parseInputToGrid(const std::string& input);

// Settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;
bool startSim = false;
bool isSpacePressed = false;
Grid grid;
//std::string input;
//std::string input = "6 5 4\n######\n#..ab#\n#..xx#\n#ooyy.\n######";
//std::string input = "8 8 12\n"
//                    "########\n"
//                    "#xab#..#\n"
//                    "#y..xab#\n"
//                    "#xooy...\n"
//                    "#y.xabx#\n"
//                    "#aby..y#\n"
//                    "#ababcz#\n"
//                    "########";
//
//std::string input = "8 8 60\n"
//                    "########\n"
//                    "#xab#..#\n"
//                    "#y..xab#\n"
//                    "#xooy...\n"
//                    "#y.xabx#\n"
//                    "#aby..y#\n"
//                    "#ababcz#\n"
//                    "########";

//std::string input = "8 8 60\n"
//                    "########\n"
//                    "#.abcab#\n"
//                    "#abxabx#\n"
//                    "#ooy..y.\n"
//                    "#xabc.z#\n"
//                    "#y..xab#\n"
//                    "#z..yab#\n"
//                    "########";
//std::string input = "8 8 10\n"
//                    "########\n"
//                    "#xab#..#\n"
//                    "#y...ab#\n"
//                    "..oo...#\n"
//                    "#..xabx#\n"
//                    "#aby..y#\n"
//                    "#ababcz#\n"
//                    "########";

//std::string input = "14 8 5\n"
//                    "##############\n"
//                    "#xab.........#\n"
//                    "#y...ab......#\n"
//                    "#xoo..x.......\n"
//                    "#yabx.y......#\n"
//                    "#abxy.z......#\n"
//                    "#abyabc......#\n"
//                    "##############";
//std::string input = "5 19 100\n"
//                    "###.#\n"
//                    "#...#\n"
//                    "#...#\n"
//                    "#...#\n"
//                    "#...#\n"
//                    "#...#\n"
//                    "#...#\n"
//                    "#xab#\n"
//                    "#y..#\n"
//                    "#zab#\n"
//                    "#...#\n"
//                    "#xab#\n"
//                    "#y.o#\n"
//                    "#xxo#\n"
//                    "#yyo#\n"
//                    "#xz.#\n"
//                    "#yux#\n"
//                    "#aby#\n"
//                    "#####";
//std::string input = "8 8 42\n"
//                    "########\n"
//                    "##abxxx#\n"
//                    "#..xyyy#\n"
//                    "#ooy.x..\n"
//                    "#xxaby.#\n"
//                    "#yy.xz.#\n"
//                    "#ab.y..#\n"
//                    "########";



// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec2 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
                                 "}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0f);\n"
                                   "}\n\0";

int main() {
//    std::string line;
//    std::stringstream input;
//
//    // Reading the first line which contains dimensions or other info.
//    std::getline(std::cin, line);
//    input << line << "\n";
//
//    // Parse the first line to know how many more lines to read for the grid.
//    int rows, cols, extraInfo;
//    std::istringstream(line) >> cols >> rows >> extraInfo; // Adjust according to your input format.
//
//    // Reading the grid lines based on the number of rows.
//    for(int i = 0; i < rows; ++i) {
//        std::getline(std::cin, line);
//        input << line << "\n";
//    }


    srand((unsigned) time(NULL));
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Move> moves;
    grid.parseInputToGrid(input);
    Solver solver = Solver(&grid);
    moves = solver.solveDFS();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " s\n";
    grid.parseInputToGrid(input);
    solver.aStar.movesListToOutput(moves);
//    return 0;
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Uncomment this statement for Mac OS X
#endif

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grid Visualization", NULL, NULL);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);


    // GLAD: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile our shader program
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data (and buffer(s)) and configure vertex attributes for the grid
    float vertices[] = {
            // Define vertices for your grid cells here
            // Placeholder for a single quad (
            // two triangles forming a rectangle)
            0.5f,  0.5f,  // top right
            0.5f, -0.5f,  // bottom right
            -0.5f, -0.5f, // bottom left
            -0.5f,  0.5f  // top left
    };
    unsigned int indices[] = {
            // Note: We start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Parse input to grid (Add your input string here)



    float verticalCarWidthScale = 0.8f; // Scale factor for the width of vertical cars
    float horizontalCarHeightScale = 0.8f;
    float carWidthOffsetScale = 0.1f;
    float carHeightOffsetScale = 0.1f;

    int frame = 0;
    Move move;
    // Render loop
    int moveCountWon = 0;
    grid.win = false;
    while (!glfwWindowShouldClose(window)) {


        if ((frame + 1)%2 == 0 && !grid.isWon() && startSim){

            if (moveCountWon < moves.size()){
//                std::cout << "xD\n";
                move = moves[moveCountWon];
                grid.moveCar(move.x, move.y, move.n, move.dir);
                moveCountWon++;
            }
//            std::vector<Car> carsThatCanMove = grid.getCarsThatCanMove();
//            grid.getPossibleMoves();
//
//            int randomCarIndex = std::rand() % carsThatCanMove.size();
//            Car& selectedCar = carsThatCanMove[randomCarIndex];
//            std::vector<int> validIndices;
//            for (size_t i = 0; i < selectedCar.northEastSouthWest.size(); ++i) {
//                if (selectedCar.northEastSouthWest[i] > 0) {
//                    validIndices.push_back(i);
//                }
//            }
//
//            int randomMoveIndex = validIndices[std::rand() % validIndices.size()];
////            std::cout << randomMoveIndex << "\n";
//
//            switch (randomMoveIndex) {
//                case 0:
//                    grid.moveCar(selectedCar.x, selectedCar.y, fmin(std::rand() %(selectedCar.northEastSouthWest[randomMoveIndex]) + 1, selectedCar.northEastSouthWest[randomMoveIndex]), MINUS);
//                    break;
//                case 1:
//                    grid.moveCar(selectedCar.x+selectedCar.length-1, selectedCar.y, fmin(std::rand() %(selectedCar.northEastSouthWest[randomMoveIndex]) + 1, selectedCar.northEastSouthWest[randomMoveIndex]), PLUS);
//                    break;
//                case 2:
//                    grid.moveCar(selectedCar.x, selectedCar.y+selectedCar.length-1, fmin(std::rand() %(selectedCar.northEastSouthWest[randomMoveIndex]) + 1, selectedCar.northEastSouthWest[randomMoveIndex]), PLUS);
//                    break;
//                case 3:
//                    grid.moveCar(selectedCar.x, selectedCar.y, fmin(std::rand() %(selectedCar.northEastSouthWest[randomMoveIndex]) + 1, selectedCar.northEastSouthWest[randomMoveIndex]), MINUS);
//                    break;
//                default:
//                    break;
//            }
//            std::cout << selectedCar.getInfo();
//            std::cout << "Moved car " << selectedCar.ID << " nesw " << randomMoveIndex << " n "<< selectedCar.northEastSouthWest[randomMoveIndex] << "\n";

        }

        // Input
        processInput(window);

        // Render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the grid
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // no need to bind every time for this simple example, but doing so for completeness

        float cellWidth = 2.0f / grid.width; // OpenGL width is 2 units (-1 to 1)
        float cellHeight = 2.0f / grid.height; // OpenGL height is 2 units (-1 to 1)

// For each cell in the grid
        for (int i = 0; i < grid.height; ++i) {
            for (int j = 0; j < grid.width; ++j) {
                float currentWidthScale = 1.0f;
                float currentHeightScale = 1.0f;
                float currentWidthOffset = 0.0f;
                float currentHeightOffset = 0.0f;

// Adjust scale based on cell type and orientation
                if (grid.cells[i][j].cellType == CAR || grid.cells[i][j].cellType == DEANSCAR) {
                    if (grid.cells[i][j].isVertical) {
                        currentWidthScale = verticalCarWidthScale;
                        if (grid.cells[i][j].isNeighbouring == PLUS){
                            currentHeightOffset = -carHeightOffsetScale;
                        }
                        else if (grid.cells[i][j].isNeighbouring == MINUS){
                            currentHeightOffset = carHeightOffsetScale;
                        }
                    } else {
                        currentHeightScale = horizontalCarHeightScale;
                        if (grid.cells[i][j].isNeighbouring == PLUS){
                            currentWidthOffset = carWidthOffsetScale;
                        }
                        else if (grid.cells[i][j].isNeighbouring == MINUS){
                            currentWidthOffset = -carWidthOffsetScale;
                        }
                    }
                }

// Calculate adjusted cell width and height
                float adjustedCellWidth = cellWidth * (currentWidthScale + currentWidthOffset);
                float adjustedCellHeight = cellHeight * (currentHeightScale + currentHeightOffset);

// Calculate cell position in OpenGL coordinates (no changes here)
                float x = -1.0f + j * cellWidth + cellWidth / 2.0f+ (currentWidthOffset) * cellWidth;
                float y = 1.0f - i * cellHeight - cellHeight / 2.0f + (currentHeightOffset) * cellHeight;

// Generate vertex data for the cell with adjusted width and height
                float vertices[] = {
                        x + adjustedCellWidth / 2, y + adjustedCellHeight / 2, // top right
                        x + adjustedCellWidth / 2, y - adjustedCellHeight / 2, // bottom right
                        x - adjustedCellWidth / 2, y - adjustedCellHeight / 2, // bottom left
                        x - adjustedCellWidth / 2, y + adjustedCellHeight / 2  // top left
                };
                // Calculate cell position in OpenGL coordinates


                // Update vertex buffer object (VBO) with the cell's vertex data
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                // Set color based on cell type
                glm::vec3 color;
                switch (grid.cells[i][j].cellType) {
                    case EMPTY:
                        color = glm::vec3(1.0f, 1.0f, 1.0f); // White
                        break;
                    case BARRIER:
                        if (grid.win){
                            color = glm::vec3(sin(frame / 10.f)/4+0.5f, 0, sin(((frame+50) / 15.f))/4+0.5f); // Gray

                        }else {
                            color = glm::vec3(0.5f, 0.5f, 0.5f); // Gray
                        }
                        break;
                    case CAR:
                        color = glm::vec3(0.0f, 0.0f, 1.0f);// blue
                        break;
                    case DEANSCAR:
                        color = glm::vec3(1.0f, 0.0f, 0.0f); // red
                        break;
                    default:
                        color = glm::vec3(1.0f, 1.0f, 1.0f); // Default to white
                }

                // Set the color uniform
                glUniform3fv(glGetUniformLocation(shaderProgram, "ourColor"), 1, &color[0]);

                // Draw the cell as a rectangle
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



                const float slimRectangleWidth = cellWidth / 10;  // Adjust for desired thickness
                const float slimRectangleHeight = cellHeight / 4; // Adjust for desired length
                const float diamondSize = std::min(cellWidth, cellHeight) / 20; // Keep the diamond size proportional to cell size

                for (int i = 0; i < grid.height; ++i) {
                    for (int j = 0; j < grid.width; ++j) {
                        float cellX = -1.0f + j * cellWidth + cellWidth / 2.0f;
                        float cellY = 1.0f - i * cellHeight - cellHeight / 2.0f;

                        std::array<std::pair<glm::vec2, bool>, 4> directions = {{
                                                                                        {{cellX, cellY + cellHeight / 4}, grid.cells[i][j].northEastSouthWest[0] > 0}, // North
                                                                                        {{cellX + cellWidth / 4, cellY}, grid.cells[i][j].northEastSouthWest[1] > 0},  // East
                                                                                        {{cellX, cellY - cellHeight / 4}, grid.cells[i][j].northEastSouthWest[2] > 0}, // South
                                                                                        {{cellX - cellWidth / 4, cellY}, grid.cells[i][j].northEastSouthWest[3] > 0}   // West
                                                                                }};

                        for (const auto& dir : directions) {
                            glm::vec2 pos = dir.first;
                            bool isRectangle = dir.second;

                            if (isRectangle) {
                                // Draw a slim rectangle
                                float rectVertices[] = {
                                        // Depending on the direction, adjust these vertices
                                        pos.x + slimRectangleWidth / 2, pos.y + slimRectangleHeight / 2,
                                        pos.x + slimRectangleWidth / 2, pos.y - slimRectangleHeight / 2,
                                        pos.x - slimRectangleWidth / 2, pos.y - slimRectangleHeight / 2,
                                        pos.x - slimRectangleWidth / 2, pos.y + slimRectangleHeight / 2,
                                };
                                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                                glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
                            } else {
                                // Draw a diamond
                                float diamondVertices[] = {
                                        pos.x, pos.y + diamondSize / 2, // Top
                                        pos.x + diamondSize / 2, pos.y, // Right
                                        pos.x, pos.y - diamondSize / 2, // Bottom
                                        pos.x - diamondSize / 2, pos.y, // Left
                                };
                                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                                glBufferData(GL_ARRAY_BUFFER, sizeof(diamondVertices), diamondVertices, GL_STATIC_DRAW);
                            }

                            // Set the color (adjust as needed)
                            glm::vec3 shapeColor = isRectangle ? glm::vec3(0.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 0.0f); // Black for rectangle, Red for diamond

                            glUniform3fv(glGetUniformLocation(shaderProgram, "ourColor"), 1, &shapeColor[0]);

                            // Draw the shape
                            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                        }
                    }
                }
            }
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
        frame++;
    }

    // Optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // GLFW: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
    // This needs to persist across calls

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Check if spacebar is pressed and was not pressed before
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!isSpacePressed) {  // First frame where space is detected as pressed
            startSim = !startSim;
            std::cout << "startSim " << startSim << "\n"; // Toggle the simulation state
            isSpacePressed = true;  // Mark as pressed
        }
    } else {
        isSpacePressed = false;  // Space is not pressed, reset the flag
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        double normalizedX = (xpos / SCR_WIDTH) * 2.0 - 1.0;
        double normalizedY = 1.0 - (ypos / SCR_HEIGHT) * 2.0;

        int gridX = (normalizedX + 1.0) / (2.0 / grid.width);
        int gridY = (1.0 - normalizedY) / (2.0 / grid.height);

        if (gridX >= 0 && gridX < grid.width && gridY >= 0 && gridY < grid.height) {
            grid.moveCar(gridX, gridY, 1, grid.cells[gridY][gridX].isNeighbouring == MINUS ? PLUS : MINUS);
            std::vector<unsigned int> hash = grid.toIntegersBaseFour();
            for (unsigned int value : hash) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
            std::cout << grid.getGridFromHashBaseFour(hash) << "\n";
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        double normalizedX = (xpos / SCR_WIDTH) * 2.0 - 1.0;
        double normalizedY = 1.0 - (ypos / SCR_HEIGHT) * 2.0;

        int gridX = (normalizedX + 1.0) / (2.0 / grid.width);
        int gridY = (1.0 - normalizedY) / (2.0 / grid.height);

        if (gridX >= 0 && gridX < grid.width && gridY >= 0 && gridY < grid.height) {
            const auto& cell = grid.cells[gridY][gridX];
            std::cout << cell.getInfo() << std::endl;
            std::cout << grid.cars[cell.carID].getInfo() << std::endl;
//            std::vector<unsigned int> hash = grid.toIntegers();
//            grid.printGridFromHash(hash, grid.width, grid.height);
            std::vector<unsigned int> hash = grid.toIntegersBaseFour();
            for (unsigned int value : hash) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
            grid.printGridFromHashBaseFour(hash);
//            for (int i = 0; i < grid.carCount; ++i) {
//                std::cout << grid.cars[i].getInfo() << std::endl;
//            }
        }
    }else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        double normalizedX = (xpos / SCR_WIDTH) * 2.0 - 1.0;
        double normalizedY = 1.0 - (ypos / SCR_HEIGHT) * 2.0;

        int gridX = (normalizedX + 1.0) / (2.0 / grid.width);
        int gridY = (1.0 - normalizedY) / (2.0 / grid.height);

        if (gridX >= 0 && gridX < grid.width && gridY >= 0 && gridY < grid.height) {
            const auto& cell = grid.cells[gridY][gridX];
            grid.moveCar(gridX, gridY, 2, grid.cells[gridY][gridX].isNeighbouring == MINUS ? PLUS : MINUS);
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

