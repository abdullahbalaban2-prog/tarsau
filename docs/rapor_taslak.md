# Sistem Programlama - Tarsau Proje Raporu

## Giriş
Bu rapor, Sistem Programlama dersi kapsamında geliştirilen **tarsau** adlı komut satırı arşivleme uygulamasının tasarım ve gerçekleştirim detaylarını içermektedir.

## Projenin Amacı
Tarsau, `tar` benzeri çalışan, verilen dosyaları sıkıştırma yapmadan tek bir dosya (`.sau`) içerisinde birleştiren ve istendiğinde bu dosyaları orijinal izinleri ve boyutları ile geri çıkaran bir uygulamadır. Bu projenin amacı, C dilinde dosya sistemi fonksiyonlarının (stat, mkdir, chmod, fopen vs.) etkin kullanımını ve POSIX uyumlu güvenli kod geliştirmeyi öğrenmektir.

## Kullanılan Dosya Yapısı
Proje, modüler programlama prensiplerine uygun olarak klasörlere ayrılmıştır:
- `include/tarsau.h`: Genel yapı tanımlamaları (struct), sabitler ve fonksiyon prototipleri.
- `src/main.c`: Komut satırı argümanlarının ayrıştırılması.
- `src/archive.c`: `-b` (arşivleme) işlemlerinin gerçekleştirilmesi.
- `src/extract.c`: `-a` (arşivi açma) işlemlerinin gerçekleştirilmesi.
- `src/validate.c`: Dosya doğrulama (ASCII kontrolü, boyut ve sayı limitleri) işlemleri.
- `src/utils.c`: İzin okuma, basename alma ve klasör oluşturma gibi yardımcı fonksiyonlar.
- `Makefile`: Projenin kolay derlenebilmesi için otomasyon scripti.
- `test/`: Projenin doğrulamasını yapmak için kullanılan örnek dosyalar.

## Arşiv Oluşturma Algoritması (`-b`)
1. Girilen argümanlar parse edilir. Dosya adeti ve boyut sınırları kontrol edilir.
2. Tüm dosyaların sadece ASCII karakterler içerip içermediği kontrol edilir. Uygun olmayan dosya varsa `[dosya] giriş dosyasının formatı uyumsuzdur!` hatası verilip çıkılır.
3. Her dosyanın `stat` ile izin (permission) ve boyut (size) bilgileri okunur.
4. Dosya başlıkları (metadata) hesaplanarak `|dosya_adi,izin,boyut|` formatında birleştirilir.
5. .sau dosyasının ilk 10 baytına metadatanın toplam boyutu yazılır.
6. Ardından metadata yazılır.
7. Son olarak, her dosya tek tek okunup içerikleri hiçbir ayraç kullanılmadan arka arkaya `.sau` dosyasına eklenir.

## Arşiv Açma Algoritması (`-a`)
1. Hedef arşiv dosyası `.sau` uzantılı mı diye kontrol edilir.
2. Dosya okunarak ilk 10 bayttan metadata bölümünün büyüklüğü alınır. Eğer dosya bozuksa `Arşiv dosyası uygunsuz veya bozuk!` hatası verilir.
3. Metadata okunur ve `|` ile `,` karakterlerine göre parse edilerek dosya bilgileri (ad, izin, boyut) belleğe yüklenir.
4. Hedef dizin (verilmişse) `mkdir` ile oluşturulur (göreceli veya mutlak yol desteklenir, örn: `d1/d2/d3`). Path traversal zafiyetine karşı dosya adlarında sadece basename kullanılır.
5. Arşivlenmiş dosya verileri boyut bilgisi kadar byte okunarak hedef dizinde yeni dosyalar olarak oluşturulur.
6. `chmod` fonksiyonuyla arşivleme öncesindeki izinleri ayarlanır.

## Kullanılan Önemli Fonksiyonlar
- `fopen`, `fread`, `fwrite`: Dosya içeriğini byte bazlı okuyup yazmak için.
- `stat`: Dosyanın izinlerini ve boyutunu tespit etmek için.
- `mkdir`: Arşiv çıkartılırken hedef dizinleri oluşturmak için.
- `chmod`: Orijinal dosya izinlerini geri yüklemek için.
- `snprintf`, `strtok`: Metadata alanını düzenlemek ve parçalamak için.

## Test Senaryoları
Uygulama, farklı senaryolara karşı test edilmiştir:
- Sadece ASCII karakterler içeren `t1.txt` vb. dosyalarla arşivleme başarıyla tamamlanmış ve `Dosyalar birleştirildi.` mesajı alınmıştır.
- Çıkarma işleminde oluşturulan `output` dizininde dosyalar hatasız açılmış, içeriklerin doğruluğu `diff` komutuyla teyit edilmiştir.
- İçerisinde ASCII dışı veriler olan `invalid.bin` eklendiğinde başarıyla yakalanmış ve program düzgün mesaj vererek çıkmıştır.
- Fazla dosya (32'den fazla) veya 200 MB sınırı gibi uç testler kod yapısında desteklenerek sınırlandırılmıştır.

## GitHub Geliştirme Süreci
Bu proje GitHub üzerinde aşağıdaki adımlarla aşamalı olarak geliştirilmiştir:

| Tarih | Commit Mesajı | Yapılan İşlem |
| :--- | :--- | :--- |
| 10.05.2026 | initial project structure | Proje klasör yapısı, Makefile ve temel dosyalar oluşturuldu. |
| 12.05.2026 | implement command parser | -b ve -a parametrelerinin ayrıştırılması eklendi. |
| 14.05.2026 | implement archive creation | .sau arşiv oluşturma algoritması yazıldı. |
| 16.05.2026 | implement archive extraction | .sau arşiv açma algoritması yazıldı. |
| 18.05.2026 | add validation and error handling | ASCII kontrolü, dosya sayısı, toplam boyut ve hata mesajları eklendi. |
| 20.05.2026 | add tests and documentation | Test dosyaları, README ve rapor taslağı güncellendi. |
| 21.05.2026 | final cleanup and report updates | Kod temizliği, son testler ve rapor düzenlemeleri yapıldı. |

## Sonuç
Proje başarıyla tamamlanmış, POSIX uyumlu bir arşivleme aracının temel çalışma prensipleri uygulanmıştır. Hata yönetimi, güvenli kodlama teknikleri ve modüler mimari kullanılarak istikrarlı bir yazılım geliştirilmiştir.

GitHub Proje Linki: https://github.com/kullanici-adi/tarsau
