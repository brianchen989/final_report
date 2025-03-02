 #include 
                    using namespace std;
                    
                    // 將索引值轉換為 Base64 字符
                    char indexToBase64Char(int index) {
                        if (index >= 0 && index <= 25) return 'A' + index; // A–Z
                        if (index >= 26 && index <= 51) return 'a' + (index - 26); // a–z
                        if (index >= 52 && index <= 61) return '0' + (index - 52); // 0–9
                        if (index == 62) return '+'; // +
                        if (index == 63) return '/'; // /
                        return '\0'; // 非法值
                    }
                    
                    // 將 Base64 字符轉換為索引值
                    int base64CharToIndex(char c) {
                        if (c >= 'A' && c <= 'Z') return c - 'A'; // A–Z
                        if (c >= 'a' && c <= 'z') return c - 'a' + 26; // a–z
                        if (c >= '0' && c <= '9') return c - '0' + 52; // 0–9
                        if (c == '+') return 62; // +
                        if (c == '/') return 63; // /
                        return -1; // 非法字符
                    }
                    
                    // Base64 編碼
                    string base64Encode(const string &input) {
                        string binaryData = "", encoded = "";
                        for (int i = 0; i < input.size(); i++) {
                            bitset<8> bits(static_cast(input[i]));
                            binaryData += bits.to_string();
                        }
                        while (binaryData.size() % 6 != 0) binaryData += "0";
                        for (int i = 0; i < binaryData.size(); i += 6) {
                            bitset<6> bits(binaryData.substr(i, 6));
                            int index = static_cast(bits.to_ulong());
                            encoded += indexToBase64Char(index);
                        }
                        while (encoded.size() % 4 != 0) encoded += '=';
                        return encoded;
                    }
                    
                    // Base64 解碼
                    string base64Decode(const string &input) {
                        string binaryData = "", decoded = "";
                        size_t padding = 0;
                        string strippedInput = input;
                    
                        // 使用 for 迴圈移除 '=' 填充字符
                        for (int i = strippedInput.size() - 1; i >= 0 && strippedInput[i] == '='; i--) {
                            strippedInput.erase(i, 1);  // 用 erase 移除 '=' 填充字符
                            padding++;
                        }
                    
                        for (int i = 0; i < strippedInput.size(); i++) {
                            int index = base64CharToIndex(strippedInput[i]);
                            if (index == -1) return "解碼失敗";
                            bitset<6> bits(index);
                            binaryData += bits.to_string();
                        }
                    
                        if (padding > 0) binaryData.erase(binaryData.end() - padding * 2, binaryData.end());
                        for (int i = 0; i < binaryData.size(); i += 8) {
                            bitset<8> bits(binaryData.substr(i, 8));
                            decoded += static_cast(bits.to_ulong());
                        }
                        return decoded;
                    }
                    
                    // 位移加密
                    string shiftEncrypt(const string &input, int k) {
                        string encrypted = "";
                        for (int i = 0; i < input.size(); i++) {
                            char c = input[i];
                            if (c >= 32 && c <= 126) {
                                char shifted = (c - 32 + k) % 95 + 32;
                                encrypted += shifted;
                            } else {
                                encrypted += c; // 非可見字符保持不變
                            }
                        }
                        return encrypted;
                    }
                    
                    // 位移解密
                    string shiftDecrypt(const string &input, int k) {
                        string decrypted = "";
                        for (int i = 0; i < input.size(); i++) {
                            char c = input[i];
                            if (c >= 32 && c <= 126) {
                                char shifted = (c - 32 - k + 95) % 95 + 32;
                                decrypted += shifted;
                            } else {
                                decrypted += c; // 非可見字符保持不變
                            }
                        }
                        return decrypted;
                    }
                    
                    // 主程式
                    int main() {
                        string mode, input;
                        int n, k;
                    
                        cout << "歡迎使用簡化加密解密系統" << endl << endl;
                        cout << "請輸入模式 (E 表示編碼, D 表示解碼): ";
                        cin >> mode;
                        cin.ignore();
                        cout << "請輸入字串: ";
                        getline(cin, input);
                        cout << "請輸入密鑰1(建議小一點): ";
                        cin >> n;
                        cout << "請輸入密鑰2: ";
                        cin >> k;
                    
                        if (mode == "E") {
                            for (int i = 0; i < n; i++) {
                                input = base64Encode(input);
                                input = shiftEncrypt(input, k);
                            }
                            cout << "加密後結果: " << input << endl;
                        } else if (mode == "D") {
                            for (int i = 0; i < n; i++) {
                                input = shiftDecrypt(input, k);
                                input = base64Decode(input);
                                if (input == "解碼失敗") {
                                    cout << "解碼失敗" << endl;
                                    return 0;
                                }
                            }
                            cout << "解碼後結果: " << input << endl;
                        } else {
                            cout << "無效的模式！請輸入 'E' 或 'D'。" << endl;
                        }
                    
                        return 0;
                    }
