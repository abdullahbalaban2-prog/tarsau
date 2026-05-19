# Tarsau Projesi

## Projenin Amacı
Tarsau, tar/rar/zip benzeri mantıkla çalışan ancak sıkıştırma yapmayan bir komut satırı arşivleme programıdır. Çoklu dosyaları tek bir arşivde toplama (`-b`) ve arşivlenmiş dosyaları geri çıkarma (`-a`) işlevlerini yerine getirir. Proje, Sistem Programlama dersi kapsamında geliştirilmiştir.

## Derleme
Proje `make` kullanılarak derlenebilir. 
Derlemek için terminalde proje kök dizinine giderek aşağıdaki komutu çalıştırın:

```bash
make
```

Temizlemek için (derlenmiş dosyaları silmek için):
```bash
make clean
```

## Kullanım Örnekleri

**Arşiv Oluşturma:**
```bash
./tarsau -b t1.txt t2.txt t3.dat -o s1.sau
```
Belirtilen giriş dosyalarını `s1.sau` adında birleştirir.

Eğer `-o` parametresi verilmezse, varsayılan dosya adı `a.sau` olur:
```bash
./tarsau -b t1.txt t2.txt
```

**Arşiv Açma:**
```bash
./tarsau -a s1.sau d1
```
`s1.sau` arşivindeki dosyaları `d1` dizini içerisine çıkarır. Dizin yoksa oluşturulur. İzinler korunur.

Eğer dizin belirtilmezse mevcut dizine çıkarılır:
```bash
./tarsau -a s1.sau
```

## Test Komutları

Uygulamanın doğruluğunu kontrol etmek için aşağıdaki komutları kullanabilirsiniz:

```bash
# Arşivleme
./tarsau -b test/t1.txt test/t2.txt test/t3.dat -o s1.sau

# Çıkarma
mkdir -p output
./tarsau -a s1.sau output

# İçerik Kontrolü
diff test/t1.txt output/t1.txt
diff test/t2.txt output/t2.txt
diff test/t3.dat output/t3.dat

# Hatalı Dosya Testi (ASCII olmayan)
./tarsau -b test/t1.txt test/invalid.bin -o bad.sau
```

## Arşiv Formatı Açıklaması
Arşiv (`.sau`) iki ana bölümden oluşur:
1. **Organizasyon / İçerik Bölümü:**
   İlk 10 bayt, bu bölümün ASCII karakterle yazılmış boyutunu belirtir (Örn: `0000000123`).
   Sonrasında `|` ile ayrılmış dosya kayıtları gelir.
   Her kayıt şu formattadır: `Dosya adı, izinler(octal), boyut|`
2. **Arşivlenmiş Dosyalar Bölümü:**
   Dosya içerikleri hiçbir ayırıcı olmadan art arda eklenir. Dosya çıkarma işlemi organizasyon bölümündeki dosya boyutları okunarak gerçekleştirilir.

## GitHub Kullanımı
Bu proje GitHub üzerinde versiyon kontrol sistemi kullanılarak aşamalı şekilde geliştirilmiştir. Geliştirme süreci boyunca anlamlı commit mesajlarıyla projenin takibi sağlanmıştır. 
