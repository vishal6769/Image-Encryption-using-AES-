#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>
#include<cstdio>
#include "aes.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

std::string pickFile() {
    std::string tempFile = "/tmp/filedialog_result.txt";

    std::string command =
        "osascript -e 'set theFile to choose file' "
        "-e 'do shell script \"echo \" & quoted form of POSIX path of theFile & \" > " + tempFile + "\"'";

    system(command.c_str());

    FILE* fp = fopen(tempFile.c_str(), "r");
    if (!fp) return "";

    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    return std::string(buffer);
}
int main() {
    std::string file = pickFile();
    
    if (!file.empty() && file.back() == '\n')
    file.pop_back();
    const char* inputFile= file.c_str();
    const char* encImg = "img_enc.png";

    int width = 0, height = 0, channels = 0;

    unsigned char* data = stbi_load(inputFile, &width, &height, &channels, 3);
    if (!data) {
        std::cerr << "Failed to load image\n";
        return 1;
    }

    int size = width * height * 3;

    // Read key
    std::string keyStr;
    std::cout << "Enter encryption key (max 16 chars): = ";
    std::cin >> keyStr;

    std::vector<uint8_t> key(keyStr.begin(), keyStr.end());
    key.resize(16, 0);

    AES a;

    // Create output buffer for encrypted pixels
    std::vector<uint8_t> encrypted(size);

    for (int i = 0; i < size; i += 16) {
        std::vector<uint8_t> block;

        for (int j = 0; j < 16 && (i + j) < size; j++)
            block.push_back(data[i + j]);

        block.resize(16, 0);

        std::vector<uint8_t> enc = a.encrypt(block, key);

        int real = std::min(16, size - i);
        std::copy(enc.begin(), enc.begin() + real, encrypted.begin() + i);
    }

    stbi_image_free(data);

    // Write PNG (RGB, 8-bit)
    if (!stbi_write_png(encImg, width, height, 3, encrypted.data(), width * 3)) {
        std::cerr << "Failed to write PNG file\n";
        return 1;
    }

    std::cout << "Encrypted PNG written: img_enc.png\n";
    return 0;
}
