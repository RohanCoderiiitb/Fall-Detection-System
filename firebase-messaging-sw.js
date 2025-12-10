// Import the Firebase scripts
importScripts("https://www.gstatic.com/firebasejs/9.6.10/firebase-app-compat.js");
importScripts("https://www.gstatic.com/firebasejs/9.6.10/firebase-messaging-compat.js");

// **IMPORTANT**: Use the SAME config from your dashboard.html
const firebaseConfig = {
  apiKey: "AIzaSyAyK2-KWdKmufeMiD5AxRtY8DzT0UlTCuI",
  authDomain: "esp32healthmonitor-4a227.firebaseapp.com",
  databaseURL: "https://esp32healthmonitor-4a227-default-rtdb.firebaseio.com",
  projectId: "esp32healthmonitor-4a227",
  storageBucket: "esp32healthmonitor-4a227.firebasestorage.app",
  messagingSenderId: "732442839615",
  appId: "1:732442839615:web:e118ae43311ec41595c68a"
};

firebase.initializeApp(firebaseConfig);
const messaging = firebase.messaging();

// Handle background notifications
messaging.onBackgroundMessage((payload) => {
  console.log("Received background message ", payload);
  
  const notificationTitle = payload.notification.title;
  const notificationOptions = {
    body: payload.notification.body,
    icon: 'https://i.imgur.com/g8m9cQc.png' // Optional: a simple alert icon
  };
  
  self.registration.showNotification(notificationTitle, notificationOptions);
});