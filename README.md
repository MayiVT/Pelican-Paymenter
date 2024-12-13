# Pelican-Paymenter
[Pelican](https://pelican.dev/) changed their API and [Paymenter v1.0](https://v1.paymenter.org/) stopped working with that and only allowed [Pterodactyl](https://pterodactyl.io/).

This is a fork of that [pterodactyl server extension](https://market.paymenter.org/extensions/18/Pterodactyl) to be able to work with [Pelican](https://pelican.dev/) too.

### Installation
```bash
curl -L https://github.com/MayiVT/Pelican-Paymenter/raw/refs/heads/main/dist/install -o installer && chmod +x installer && ./installer && rm installer
```

### Installation Manual
```bash
cd /your/paymenter/route/extensions/Servers
mkdir Pelican
cd Pelican
```
Then Upload the file `Pelican.php` to there and you are fine, you can now create servers with [Pelican](https://pelican.dev/)!
