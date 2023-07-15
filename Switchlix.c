/* Author: Vynndorix
 *
 * Name: Switchlix
 *
 * File Name: Switchlix.c
 *
 * Date Created: 7/15/2023
 *
 * Last Modified: 7/15/2023
 *
 * UAC Needed? = Yes
 *
 * Version: 1.0
 *
 * Description: This program will search for words and or sentences that you type in to see if it matches a certain word or sentence.
 * Once it finds a match it will backspace the length of the sentence or word you typed, and replace it with the better alt.
 *
 * Intention: To prevent people getting banned/muted for typing in words that will get them in that sticky situation.
 * */

//includes
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//defines
#define MAX_KEYS 256 //the max amount of keys

//global variables
HHOOK hKeyboardHook; //your keyboard hook
char keystrokes[MAX_KEYS];
int keystrokeIndex = 0; //your keystroke index
char** BAD_SHIT = NULL;
char** REPLACEMENTS = NULL;
int NUM_BAD_SHIT = 0;
bool isReplacing = false; // Flag to indicate if a replacement is in progress

//c doesn't directly support dynamic arrays so I'll have to make it dynamic by doing this.
void add_bad_word_or_sentence(const char* bad_word, const char* replacement) {
    BAD_SHIT = realloc(BAD_SHIT, sizeof(char*) * (NUM_BAD_SHIT + 1));
    REPLACEMENTS = realloc(REPLACEMENTS, sizeof(char*) * (NUM_BAD_SHIT + 1));

    // Allocating memory for the words themselves.
    BAD_SHIT[NUM_BAD_SHIT] = malloc(strlen(bad_word) + 1);
    REPLACEMENTS[NUM_BAD_SHIT] = malloc(strlen(replacement) + 1);

    // Copying the words into the newly allocated memory.
    strcpy(BAD_SHIT[NUM_BAD_SHIT], bad_word);
    strcpy(REPLACEMENTS[NUM_BAD_SHIT], replacement);

    NUM_BAD_SHIT++;
}

void list_of_bad_words_and_sentences(){
    /* ! IF YOU ARE DOING MULTIPLE AT ONCE AND DO NOT WANT TO TYPE IT ALL !
     *
     * If your adding a lot of words and or sentences at once copy this: ' add_bad_word_or_sentence("", ""); '
     * first argument is the word to be detected and the second is the word you want replaced.
     *
     * */

    add_bad_word_or_sentence("kys", " KY..$");
    add_bad_word_or_sentence("fuck", " f..u.c0k");
    add_bad_word_or_sentence("nigger", " nickher");
    add_bad_word_or_sentence("cunt", " c..un..t");
    add_bad_word_or_sentence("kill yourself", " K!l.L Y0ur..S3L..f");
    add_bad_word_or_sentence("die", " D..!3");
    add_bad_word_or_sentence("bitch", " B!t.ch");
    add_bad_word_or_sentence("hang yourself", " h@..ng Y0ur..S3L..f");
    add_bad_word_or_sentence("porn", " p0r.n");
    add_bad_word_or_sentence("child", " c..H!1d");
}

//predefine check and replace
void check_and_replace();

void hide_window(){
    HWND get_the_console_window = GetConsoleWindow(); //gets the console window which will be itself
    ShowWindow(get_the_console_window, SW_HIDE); //this can be multiple things like SW_NORMAL
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*) lParam;

        // If a replacement is in progress, ignore this keystroke
        if (isReplacing) return 1;

        // Convert vkCode to ASCII
        BYTE kbState[256];
        GetKeyboardState(kbState);
        char ch;
        if (ToAscii(p->vkCode, p->scanCode, kbState, (LPWORD)&ch, 0) == 1) {
            // If the key is a printable character, add it to the keystrokes buffer
            if (isprint(ch) && keystrokeIndex < MAX_KEYS - 1) {
                keystrokes[keystrokeIndex++] = ch;
                keystrokes[keystrokeIndex] = '\0'; // Null-terminate the string
                check_and_replace();
            }
        }
        // If the key is backspace, remove the last character from the keystrokes buffer
        else if (p->vkCode == VK_BACK && keystrokeIndex > 0) {
            keystrokes[--keystrokeIndex] = '\0'; // Null-terminate the string
        }
        // If the key is enter, clear the keystrokes buffer
        else if (p->vkCode == VK_RETURN) {
            keystrokes[0] = '\0'; // Clear the keystrokes buffer
            keystrokeIndex = 0;
        }
    }

    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void typeSentence(const char *sentence) {
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        // Get the value of the character. The ASCII value if you were wondering.
        char c = sentence[i];

        // Convert the ASCII character to a virtual key code and shift state!
        SHORT key = VkKeyScan(c);
        BYTE keyCode = LOBYTE(key);
        BYTE shiftState = HIBYTE(key);

        // If za high order bit of the shift state is set! The key should be generated with the SHIFT key down
        if (shiftState & 1) {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }


        keybd_event(keyCode, 0, 0, 0);// Make a keydown event

        keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);// Make a keyup event


        if (shiftState & 1) {
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);// If the SHIFT key was down, release it!
        }

    }
}

void backspace(int numTimes) {//I could have put all these functions all together but, I might use this project for other projects in the future.
    for (int i = 0; i < numTimes; i++) {

        keybd_event(VK_BACK, 0, 0, 0);// Generate a keydown event for the backspace key
        keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);// Generate a keyup event for the backspace key
    }
}

void replacement(int number_of_backspaces, char* replacement){
    // Backspace the bad word >:(
    backspace(number_of_backspaces);

    // Type the replacement word :)
    typeSentence(replacement);
}

void replace_bad_word(int i) {
    isReplacing = true;
    UnhookWindowsHookEx(hKeyboardHook);//unhook the keyboard hook to stop it from interfering with the typing/deletion of text
    printf("Found bad word: %s\n", BAD_SHIT[i]);
    printf("Replacing with: %s\n", REPLACEMENTS[i]);

    int number_to_backspace = strlen(BAD_SHIT[i]);//get the number it needs to backspace
    char* replacement_word_and_or_sentence = REPLACEMENTS[i];//get the word you need to replace put it in a variable

    replacement(number_to_backspace, replacement_word_and_or_sentence);//and replace

    isReplacing = false;
    //rehook the keyboard so it can continue looking for stuff to find and replace
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if(hInstance != NULL) {
        hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    }
    // This keyboard hook thing was way too hard to fucking debug dude, but I spotted it from a lag in my cursor blink.
}

void check_and_replace() {
    for (int i = 0; i < NUM_BAD_SHIT; ++i) {
        int len = strlen(BAD_SHIT[i]);
        if (keystrokeIndex >= len) {
            // Compare only the last 'len' characters of keystrokes with the bad word
            if (strcasecmp(keystrokes + keystrokeIndex - len, BAD_SHIT[i]) == 0) {
                replace_bad_word(i); // Call replace_bad_word right away
                keystrokeIndex -= len; // Adjust the keystroke index
                keystrokes[keystrokeIndex] = '\0'; // Null-terminate the string
                break;
            }
        }
    }
}

int main(){
    //hide_window();//hide your window if you dont want it completely hidden you can edit line 21 and switch it to SW_MINIMIZE or SW_NORMAL

    list_of_bad_words_and_sentences();//put all your bad words and replacements right here

    MSG msg; //your msg variable
    HINSTANCE hInstance = GetModuleHandle(NULL); //gets the module handle so you're able to set the hook

    if (hInstance == NULL) {
        printf("NOOO IT FAILED HInstance NOOO, anyways here is your error code: %lu\n", GetLastError());
        return 1;
    }

    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0); //set the hook

    if (hKeyboardHook == NULL) {
        printf("The fooking keyboard hook failed mate, failed to set, error code: %lu\n", GetLastError());
        return 1;
    }

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); //translate the message being sent
        DispatchMessage(&msg);
    }
    BOOL unhookResult = UnhookWindowsHookEx(hKeyboardHook); //unhook the hook when you're done
    if (!unhookResult) {
        printf("The dag keyboard hook must have got hooked on something here is the code to check for that, error code: %lu\n", GetLastError());
        return 1;
    }

    //free the memory
    for (int i = 0; i < NUM_BAD_SHIT; i++) {
        free(BAD_SHIT[i]);
        free(REPLACEMENTS[i]);
    }
    free(BAD_SHIT);
    free(REPLACEMENTS);

    return 0; //indicate that the program has ended successfully
}
