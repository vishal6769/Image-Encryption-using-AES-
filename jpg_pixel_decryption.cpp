#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include<cstdio>
#include <algorithm>

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
    const char* encImg = file.c_str();
    const char* decImg = "img_dec_out.png";

    int width = 0, height = 0, channels = 0;

    // Load encrypted PNG (RGB)
    unsigned char* data = stbi_load(encImg, &width, &height, &channels, 3);
    if (!data) {
        std::cout << "Failed to load encrypted PNG\n";
        return 1;
    }

    int size = width * height * 3;

    // Read key
    std::string keyStr;
    std::cout << "Enter decryption key (must match encryption key): = ";
    std::cin >> keyStr;

    std::vector<uint8_t> key(keyStr.begin(), keyStr.end());
    key.resize(16, 0);

    AES a;

    std::vector<uint8_t> decrypted(size);

    // AES block decrypt
    for (int i = 0; i < size; i += 16) {
        std::vector<uint8_t> block;

        // load up to 16 bytes
        for (int j = 0; j < 16 && (i + j) < size; j++)
            block.push_back(data[i + j]);

        block.resize(16, 0);

        std::vector<uint8_t> dec = a.decrypt(block, key);

        int real = std::min(16, size - i);
        std::copy(dec.begin(), dec.begin() + real, decrypted.begin() + i);
    }

    stbi_image_free(data);

    // Save restored pixels to PNG
    if (!stbi_write_png(decImg, width, height, 3, decrypted.data(), width * 3)) {
        std::cout << "Failed to write decrypted PNG\n";
        return 1;
    }

    std::cout << "Decryption complete! Output: img_dec_out.png\n";
    return 0;
}
