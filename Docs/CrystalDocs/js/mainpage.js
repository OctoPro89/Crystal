const letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
document.querySelector("h1").onmouseover = event => {
  let iterations = 0;
  const interval = setInterval(() => {
  event.target.innerText = event.target.innerText.split("")
    .map(letter => letters[Math.floor(Math.random() * 26)])
    .join("");
    
   if(iterations >= 9) clearInterval(interval);
    
   iterations += 1;
}, 30);
}