export function addTime(date, timeStr) {
    const [hours, minutes, seconds] = timeStr.split(":").map(Number);
    const result = new Date(date);
    result.setHours(result.getHours() + hours, result.getMinutes() + minutes, result.getSeconds() + seconds);
    return result;
}
