function fetchLatestData() {
    fetch(window.location.href + '?latest=1')
        .then(response => response.json())
        .then(data => {
            if (!data.error) {
                document.getElementById('temperature').textContent = data.nhiet_do;
                document.getElementById('humidity').textContent = data.do_am;
                document.getElementById('timestamp').textContent = data.created_at;
            } else {
                console.error("No data found");
            }
        })
        .catch(error => console.error('Error fetching data:', error));
}

function timedRefresh(timeoutPeriod) {
    setTimeout(function () {
        location.reload(true);
    }, timeoutPeriod);
}

window.onload = function () {
    if (document.getElementById('temperature')) {
        fetchLatestData();
        timedRefresh(5000); // Refresh every 5 seconds
    }
}