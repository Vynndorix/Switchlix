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

//defines
#define MAX_KEYS 256 //the max amount of keys
#define _CRT_SECURE_NO_WARNINGS//To remove warnings from the visual studio compiler

//includes
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//global variables
HHOOK hKeyboardHook; //your keyboard hook
char keystrokes[MAX_KEYS];
int keystrokeIndex = 0; //your keystroke index
char** BAD_SHIT = NULL;
char** REPLACEMENTS = NULL;
int NUM_BAD_SHIT = 0;
bool isReplacing = false; // Flag to indicate if a replacement is in progress

//c doesn't directly support dynamic arrays so, I'll have to make it dynamic by doing this.
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

    add_bad_word_or_sentence("kys", " KY..$ ");
    add_bad_word_or_sentence("fuck", " f..u.c0k ");
    add_bad_word_or_sentence("nigger", " nickher ");
    add_bad_word_or_sentence("cunt", " c..un..t ");
    add_bad_word_or_sentence("kill", " K!l.L ");
    add_bad_word_or_sentence("yourself", " Y0ur..S3L..f ");
    add_bad_word_or_sentence("die", " D..!3 ");
    add_bad_word_or_sentence("bitch", " B!t.ch ");
    add_bad_word_or_sentence("hang yourself", " h@..ng Y0ur..S3L..f ");
    add_bad_word_or_sentence("porn", " p0r.n ");
    add_bad_word_or_sentence("child", " c..H!1d ");
    add_bad_word_or_sentence("slut", " S..L.u1 ");
    add_bad_word_or_sentence("nigga", " ne3gah ");
    add_bad_word_or_sentence("stfu",  "S..T,fu ");
    add_bad_word_or_sentence("kike", " ki.,k3 ");
    add_bad_word_or_sentence("retard", " r3..t@_rd ");
    add_bad_word_or_sentence("I hope", "I H0.p3 ");
    add_bad_word_or_sentence("I wish", "I W,,.iS--h ");
    add_bad_word_or_sentence("anal", " aN^^a-l ");
    add_bad_word_or_sentence("bastard", " B@sT..ar-d ");
    add_bad_word_or_sentence("blowjob", " bl0wj..0b ");
    add_bad_word_or_sentence("cock", " c0..c,K ");
    add_bad_word_or_sentence("clitoris", "c_l.1t0r1s ");
    add_bad_word_or_sentence("coon", " c00..n ");
    add_bad_word_or_sentence("goon", " g00..n ");
    add_bad_word_or_sentence("dyke", " d..yk3 ");
    add_bad_word_or_sentence("fag", " f@..g ");
    add_bad_word_or_sentence("faggot", "fa@@0..t ");
    add_bad_word_or_sentence("felching", " fe^^lc..h_ing ");
    add_bad_word_or_sentence("dildo", " D1l..d0 ");
    add_bad_word_or_sentence("damn", " d@..M^n ");
    add_bad_word_or_sentence("Goddamn", " G0d^^D@..M^n ");
    add_bad_word_or_sentence("G0d D@..m(n", " G0d&&D@,,M.n ");
    add_bad_word_or_sentence("hell", " h3.L1 ");
    add_bad_word_or_sentence("homo", " H0..m,0 ");
    add_bad_word_or_sentence("jerk", " j3r^k ");
    add_bad_word_or_sentence("jizz", " ji^zz ");
    add_bad_word_or_sentence("piss", " pi..^ss ");
    add_bad_word_or_sentence("pube", " p.Ub,3 ");
    add_bad_word_or_sentence("prick", " pri..cK ");
    add_bad_word_or_sentence("pussy", " p.^U.,^SSy ");
    add_bad_word_or_sentence("queer", " qu..3^,3r ");
    add_bad_word_or_sentence("scrotum", " S,.C0rtu^^m ");
    add_bad_word_or_sentence("sex", " s3.,^X ");
    add_bad_word_or_sentence("smegma", " Sm3..g,.(ma ");
    add_bad_word_or_sentence("spunk", " sp^^u,n(k) ");
    add_bad_word_or_sentence("tit", " t!,.^t ");
    add_bad_word_or_sentence("tosser", "t0ss,.(er) ");
    add_bad_word_or_sentence("twat", " t,.^^w(a)^t ");
    add_bad_word_or_sentence("vagina", " va,.G(i),N,.^^A ");
    add_bad_word_or_sentence("wank", " W(A),.N*K ");
    add_bad_word_or_sentence("whore", " Wh)0,(RE) ");
    add_bad_word_or_sentence("boner", " b0^^N3r ");
    add_bad_word_or_sentence("balls", " B@L(L)$ ");
    add_bad_word_or_sentence("ass", "A__(^^S$) ");
    add_bad_word_or_sentence("beaner", " B3(A)n3(R) ");
    add_bad_word_or_sentence("black", " bL(AC),^^K ");
    add_bad_word_or_sentence("blumkin", " b(L)Umk(in ");
    add_bad_word_or_sentence("bullshit", " bu(LL),Sh^!__(T) ");
    add_bad_word_or_sentence("circlejerk", "c!R..CLe(Jer,.^k) ");
    add_bad_word_or_sentence("cum", "c_=U({M}) ");
    add_bad_word_or_sentence("creampie", "Cr3A(M){^^P^I3} ");
    add_bad_word_or_sentence("rape", " R@P{{E}} ");
    add_bad_word_or_sentence("dingle", " D(!)N{GL^e} ");
    add_bad_word_or_sentence("dong", " d0({N),..^G ");
    add_bad_word_or_sentence("eat my ass", " e(A^t,,.{M}{y}(A^^..$$) ");
    add_bad_word_or_sentence("finger", " f!N..g^^3r ");
    add_bad_word_or_sentence("fist", " f({Is),t ");
    add_bad_word_or_sentence("fetish", " fE_-(T)i$^,H ");
    add_bad_word_or_sentence("frotting", " fr(0T)){t},iN.g ");
    add_bad_word_or_sentence("gay", " g__(;a;{y} ");
    add_bad_word_or_sentence("gang", " g=a=^n^,g, ");
    add_bad_word_or_sentence("grope", " gr0..,p3 ");
    add_bad_word_or_sentence("hardcore", " h@r^d_{co}re ");
    add_bad_word_or_sentence("hentai", " hen({T}a)I ");
    add_bad_word_or_sentence("g-spot", " g^,Sp0;T ");
    add_bad_word_or_sentence("g spot", " g^,Sp0;T ");
    add_bad_word_or_sentence("guro", " g(U)R{0 ");
    add_bad_word_or_sentence("hooker", "h{00}k^e*R ");
    add_bad_word_or_sentence("horny", " h0R_+n_y} ");
    add_bad_word_or_sentence("how to kill", " h0w{{T0}}K^.L(L) ");
    add_bad_word_or_sentence("fat", " F({A}^T) ");
    add_bad_word_or_sentence("incest", " in{{{c.}e,}s(}(T) ");
    add_bad_word_or_sentence("intercourse", " in{T}3r,.,.C0r_$3 ");
    add_bad_word_or_sentence("jigaboo", " J.I,,G@B00 ");
    add_bad_word_or_sentence("jiggaboo", " J.I,,GG@B00 ");
    add_bad_word_or_sentence("jiggerboo", " J.I,,GGRB00 ");
    add_bad_word_or_sentence("kink", " K__!n,.K ");
    add_bad_word_or_sentence("jugs", " j**UG_$ ");
    add_bad_word_or_sentence("loli", " l(0)LI ");
    add_bad_word_or_sentence("masterbait", " m@St_3^rB@I(.T) ");
    add_bad_word_or_sentence("masturbation", " m@St_3^rB@(.T)I0N ");
    add_bad_word_or_sentence("make me come", " m@K3 m3 c{0}m3 ");
    add_bad_word_or_sentence("milf", " mI({;L}F) ");
    add_bad_word_or_sentence("motherfucker", " m0T..H3r^^F.U{C}kE^R ");
    add_bad_word_or_sentence("negro", " n3{G}R0 ");
    add_bad_word_or_sentence("natzi", " n(A)T,.Z(I} ");
    add_bad_word_or_sentence("nsfw", " n..$F(W) ");
    add_bad_word_or_sentence("nude", " N(U)D3 ");
    add_bad_word_or_sentence("pedo", " p3(D)0 ");
    add_bad_word_or_sentence("one cup two girls", " On3..cU^P.Tw0...{G)Ir_L$ ");
    add_bad_word_or_sentence("one man one jar", " On3 mA(N) O..N3 Ja^R ");
    add_bad_word_or_sentence("orgasm", " 0^RG{A,,Sm ");
    add_bad_word_or_sentence("orgy", " 0rg^^y ");
    add_bad_word_or_sentence("poop", " p00(P) ");
    add_bad_word_or_sentence("queaf", " Q(U)3A{F} ");
    add_bad_word_or_sentence("queef", " q={U33}}F ");
    add_bad_word_or_sentence("raping", " R@.,P__I{N}G ");
    add_bad_word_or_sentence("rapist", " R@..PI(st ");
    add_bad_word_or_sentence("semen", " S3(M)3N ");
    add_bad_word_or_sentence("rimjob", " r!M^J0^,.B ");
    add_bad_word_or_sentence("rimming", " r!{MM}i,.^ng ");
    add_bad_word_or_sentence("splooge", " spl00({g)e ");
    add_bad_word_or_sentence("strapon", " St^rA{p0N ");
    add_bad_word_or_sentence("strap on", " St^rA{p0N ");
    add_bad_word_or_sentence("swastica", " S(W@)st!Ca ");
    add_bad_word_or_sentence("tea bagging", " T3A....B{A}G^GIn(G ");
    add_bad_word_or_sentence("threesome", " Th^r33S0m3 ");
    add_bad_word_or_sentence("tranny", " Tr@Ns(F0R.,mer ");
    add_bad_word_or_sentence("vibrator", " V!BrA^Tor ");
    add_bad_word_or_sentence("vulva", " V(Ul..V@ ");
    add_bad_word_or_sentence("wetback", " W3TB@.,C;K ");
    add_bad_word_or_sentence("wet dream", " W3t,,D{R}E,A.M ");
    add_bad_word_or_sentence("white power", " W,.H{I}T3,.,.P0{W3r ");
    add_bad_word_or_sentence("whore", " wh..0R^3 ");
    add_bad_word_or_sentence("wank", " W@{N}K ");
    add_bad_word_or_sentence("xxx", "}^X^}X{x ");
    add_bad_word_or_sentence("yellow showers", " Y3LL0.W,.,.Sh0W3r$ ");
    add_bad_word_or_sentence("zoophilia", " Z00Phi^li^A ");
    add_bad_word_or_sentence("shit", " sH!_T ");
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

int strcasecmp(const char *s1, const char *s2) {//visual studio compiler was giving me shit so, I just made it myself
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
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
