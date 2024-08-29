// Hàm lấy dữ liệu cảm biến mới nhất và cập nhật giao diện
function fetchLatestSensorData() {
    fetch(window.location.href + '?latest=1')
        .then(response => response.json())
        .then(data => {
            console.log(data)
            if (!data.error) {

            document.getElementById('temperature1').textContent = data.nhiet_do1 || 'N/A';
            document.getElementById('humidityAir1').textContent = data.do_am1 || 'N/A';
            document.getElementById('temperature2').textContent = data.nhiet_do2 || 'N/A';
            document.getElementById('humidityAir2').textContent = data.do_am2 || 'N/A';
            document.getElementById('temperature3').textContent = data.nhiet_do3 || 'N/A';
            document.getElementById('humidityAir3').textContent = data.do_am3 || 'N/A';
            document.getElementById('timestamp').textContent = data.created_at || 'N/A';
        }else{
            console.error("Error fetching data:", data.error);
        }
        }
    )
    .catch(error => console.error('Error fetching data:', error));
}

// Hàm lấy tất cả dữ liệu cảm biến và hiển thị trên bảng
function fetchAllSensorData() {
    fetch(window.location.href + '?all_data')
        .then(response => response.json())
        .then(data => {
            const tableBody = document.querySelector('#sensor-data-table tbody');
            tableBody.innerHTML = ''; // Xóa dữ liệu cũ trước khi thêm mới
            data.forEach(row => {
                const tableRow = document.createElement('tr');
                tableRow.innerHTML = `
                    <td>${row.id || 'N/A'}</td>
                    <td>${row.nhiet_do1 || 'N/A'}</td>
                    <td>${row.do_am1 || 'N/A'}</td>
                    <td>${row.nhiet_do2 || 'N/A'}</td>
                    <td>${row.do_am2 || 'N/A'}</td>
                    <td>${row.nhiet_do3 || 'N/A'}</td>
                    <td>${row.do_am3 || 'N/A'}</td>
                    <td>${row.lon || 'N/A'}</td>
                    <td>${row.lat || 'N/A'}</td>
                    <td>${row.currentValue || 'N/A'}</td>
                    <td>${row.created_at || 'N/A'}</td>
                `;
                tableBody.appendChild(tableRow);
            });
        })
        .catch(error => console.error('Error fetching sensor data:', error));
}

// Hàm lặp lại gọi lấy dữ liệu mới nhất sau khoảng thời gian nhất định
function timedRefresh(timeoutPeriod) {
    setTimeout(() => {
        fetchLatestSensorData(); 
        fetchAllSensorData();// Gọi lại hàm để cập nhật dữ liệu mới nhất mà không làm mới toàn bộ trang
        timedRefresh(timeoutPeriod); // Tiếp tục lặp lại quá trình này
    }, timeoutPeriod);
}

// Chạy khi trang được tải
window.onload = function () {
    if (document.getElementById('temperature1')) {
        fetchLatestSensorData(); // Gọi hàm để lấy dữ liệu mới nhất ngay khi trang được tải
        fetchAllSensorData(); // Gọi hàm để lấy toàn bộ dữ liệu ngay khi trang được tải
        timedRefresh(5000); // Cập nhật lại dữ liệu mới nhất mỗi 5 giây
    }
};
