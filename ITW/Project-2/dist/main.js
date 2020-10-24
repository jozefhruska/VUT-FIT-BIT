/* Mobile navigation */
const navToggle = document.getElementById("nav-toggle");
const navList = document.getElementById("nav-list");

navToggle.onclick = () => {
  navList.classList.toggle("navigation__list--active");
};
/* Smooth scrolling */


const links = document.querySelectorAll("[data-target]");

for (let link of links) {
  link.onclick = function (event) {
    event.preventDefault();
    /* Hide navigation */

    navList.classList.remove("navigation__list--active");
    const targetId = this.getAttribute("data-target");
    const targetElement = document.getElementById(targetId);
    targetElement.scrollIntoView({
      alignToTop: true,
      behavior: "smooth",
      inline: "nearest"
    });
  };
}
/* Skill bars width */


const skillBars = document.getElementsByClassName("skills__bar");

for (let bar of skillBars) {
  const span = bar.children[0];
  span.style.width = span.getAttribute("data-value");
}
/* Skills tabs */


const tabControls = document.getElementsByClassName("skills__control");
const tabContents = document.getElementsByClassName("skills__content");

for (let control of tabControls) {
  control.onclick = () => {
    const target = control.getAttribute("data-tab");

    for (element of tabControls) {
      if (element.getAttribute("data-tab") !== target) {
        element.classList.remove("button--red");
        element.classList.add("button--secondary");
      } else {
        element.classList.add("button--red");
      }
    }

    for (element of tabContents) {
      if (element.id !== target) {
        element.classList.remove("skills__content--active");
      } else {
        element.classList.add("skills__content--active");
      }
    }
  };
}
//# sourceMappingURL=main.js.map
