let scrollToTopButton = document.getElementById('scroll-to-top')
let contentContainer = document.getElementsByClassName('content-container')[0]

contentContainer.onscroll = function () {
    if (contentContainer.scrollTop > 100) {
        scrollToTopButton.style.display = "block";
    } else {
        scrollToTopButton.style.display = "none";
    }
};

scrollToTopButton.onclick = function () {
    contentContainer.scrollTo({ top: 0, behavior: 'smooth' });
};