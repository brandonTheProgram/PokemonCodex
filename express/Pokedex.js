const express = require('express');
const path = require('path');
const axios = require('axios');

const app = express();
const port = 3000;

app.use(express.json());

// Serve the index.html file
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'Home.html'));
});

// Route to fetch all Pokédex images
app.get('/pokedex', async (req, res) => {
    try
    {
        const response = await axios.get('http://localhost:8080/pokedex');
        res.json(response.data);
    } catch (error) {
        console.error('Error fetching Pokédex images from database:', error);
        res.status(500).json({ error: 'Failed to fetch Pokédex images' });
    }
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
