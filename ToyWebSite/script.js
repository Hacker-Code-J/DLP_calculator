let currentInput = "";
let display = document.getElementById('display');

function input(value) {
  currentInput += value;
  display.textContent = currentInput;
}

function calculate() {
  try {
    display.textContent = eval(currentInput);
    currentInput = display.textContent;
  } catch (e) {
    display.textContent = "Error";
    currentInput = "";
  }
}

function clearDisplay() {
  currentInput = "";
  display.textContent = '0';
}
