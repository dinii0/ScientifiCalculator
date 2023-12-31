# <h2 align=center>*ScientifiCalculator*</h2>

<p align="center">
   <img src= "https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/background.jpg?raw=true">
</p>

## <h2 align=center>*What is this Calculator about?* </h2>
This calculator helps users to calculate different equations, the calculator gives you opportunity to work with matrices, GCM, sigma function, pi, sin, tan, cos etc.

## <h2 align=center>*Files and their structure* </h2>
   + [BackgroundMode.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/BackgroundMode.cpp)
        - Monitors application color schemes, all buttons, window, texts fonts and colors.
   + [CalculateButtons.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/CalculateButtons.cpp)
        - A structured data where It creates/paints all buttons that helps user to implement numbers/symbols inside the calculator.
   + [ConvertData.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/ConvertData.cpp)
        - Some convertion functions where it converts Windows.h objects into string/double and vice-versa. This way it made me easier to change strings/double values and use them inside Windows.h objects.
   + [DeviatedWindows.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/DeviatedWindows.cpp)
        - Some buttons required more variable than the others in order to achieve a result for the equation, so therefor I seperated those button's window from the others. These window ask more than 3 variables as an input.
   + [DynamicNumbers](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/DynamicNumbers.cpp)
        - Everytime client wants a number/symbol be written inside the equation this file takes the control of it, it maps the symbols and numbers for the equation (I didn't want to hardcode it because it would make the word harder for me to code it), theres also an algorithm that finds the result of the equation if its available.
   + [HistoryList.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/HistoryList.cpp)
        - Creates/paints/menages the history window and the button that saves equations inside it.
   + [MathFormulas.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/MathFormulas.cpp)
        - All formulas and algorithms are written here, we take variables (input from client) and we return the output back to the calculator.
   + [ResourceReader.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/ResourceReader.cpp)
        - Two functions I found online that helpt me use resources like png/mp3, instead of saving them in the directory and use them continously while the app is running, it saves them in my executable file in that way we don't need the files to be in my directory everytime the application starts.
   + [ScientifiCalculator](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/ScientifiCalculator.cpp)
        - The main file, this is where the main window is created and executed first.
   + [TableNumber.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/TableNumber.cpp)
        - Creates/paints the equation and the results in the window table.
   + [TitleBar.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/TitleBar.cpp)
        - Creates/paints the title bar and it created custom exit/minimaze buttons.
   + [WarningBox.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/WarningBox.cpp)
        - Creates/paints the warning window table, loads data from bass.dll library, and emits a warning sound everytime clients mistype the equation.
   + [WindowButtons.cpp](https://github.com/dinii0/ScientifiCalculator/blob/main/ScientifiCalculator/WindowButtons.cpp)
        - Second most important file (after the main one). This creates connection almost every file in order to make it work. Reads the value from user, checks it and returns to the other files to do their work or they use functions from other files to process something and return to other files.

   
## <h2 align=center>*There are 3 windows, what are those about??* </h2>
  + Number table
       - Here the app shows the equation you wrote as an input, and if there is a result it will be painted on the bottom of the window.
  + Warning table
       - Everytime you input something that is mathematical/logical incorrect, the window warns you by outputing the error and emiting a warning sound
  + Temporary save
       - If you want to save an equation later while calculating, you can press the save button (top-right), and it will be saved till u close the application.

## <h2 align=center>*What is the button next to the 'Save' one?* </h2>
I implemented a dark/light mode feature. You can change the color scheme of the application everytime you press it.

## <h2 align=center>*Installation* </h2>
Nothing difficult, just download the [file](https://github.com/dinii0/ScientifiCalculator/releases/tag/scientific.calculator), extract it somewhere in your harddisk, and run the application. (Make sure the bass.dll file is in the same directory as your .exe!)

## <h2 align=center>*Some informations regarding to the application* </h2>
- History window will save only the last **10 equations** you saved, after the that it starts discarding the old equations.
- You can make your equation as long as you want, untill you try to pass the right side of the window, but either way if you try you'll be warned with a sound and a text from warning table.
- I won't be updating this application sooner or later, unless there's a huge bug or a glitch that kills the proccess of working efficiently
- This project was tested on this system:
   + Windows 11
   + 64 bit
   + 32 GB RAM
   + Intel Proccesor
   + NVIDIA Graphics Card

... with that being said, I'm expecting on some operative system to not work or to be buggy. Especially old operating system with outdated drivers.


## <h2 align=center>*References* </h2>
   + ResourceReader.cpp: [Stackoverflow post](https://stackoverflow.com/a/66238748) and [ChatGPT](https://chat.openai.com/)
   + [Audio warning](https://pixabay.com/sound-effects/wrong-answer-126515/)
   + [Eigen](https://github.com/libigl/eigen)
   + [Bass](https://www.un4seen.com/)


<table align="center">
  <tr>
    <td align="center">
      <h2>Issues, pull requests and repo</h2>
      <a href="https://github.com/dinii0/ScientifiCalculator/issues">
        <img src="https://img.shields.io/github/issues/dinii0/ScientifiCalculator" alt="GitHub issues">
      </a>
      <a href="https://github.com/dinii0/ScientifiCalculator/issues?q=is%3Aissue+is%3Aclosed">
        <img src="https://img.shields.io/github/issues-closed/dinii0/ScientifiCalculator" alt="GitHub issues closed">
      </a>
      <a href="https://github.com/dinii0/ScientifiCalculator/pulls">
        <img src="https://img.shields.io/github/issues-pr/dinii0/ScientifiCalculator" alt="GitHub pull requests">
      </a>
      <a href="https://github.com/dinii0/ScientifiCalculator/pulls?q=is%3Apr+is%3Aclosed">
        <img src="https://img.shields.io/github/issues-pr-closed/dinii0/ScientifiCalculator" alt="GitHub pull requests closed">
      </a>
      <br>
      <img src="https://github-readme-stats.vercel.app/api?username=dinii0&show_icons=true" alt="dinii0's GitHub Stats">
      <br>
      <h2>You can give me a star!</h2>
      <a href="https://github.com/dinii0/ScientifiCalculator/stargazers">
        <img src="https://i.imgur.com/FyVXkZL.png" alt="Built with Love">
      </a>
    </td>
  </tr>
</table>

