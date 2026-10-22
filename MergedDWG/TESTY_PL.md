# Raport weryfikacji, 22.10.2026

## Błąd zgłoszony w Resolve i poprawka 1.0.1

Użytkownik zgłosił rzeczywisty błąd kompilacji wersji 1.0: „wrong argument PrimeraParams u in Transform DCTL” oraz „Transform DCTL function has wrong arguments”. Testy CPU nie wykryły ograniczenia parsera Resolve. Zmieniono nazwy pomocnicze z P/H/S/K_transform na P/H/S/K_process. Statycznie potwierdzono tylko jedno wystąpienie `transform(` w dostarczonym DCTL. Powtórzono pełny test CPU z tym samym wynikiem. Kompilacja poprawki w Resolve nie została potwierdzona.

## Wynik

**PASS w testach zastępczych CPU. Kompilacja GPU i render w DaVinci Resolve: NIE WYKONANO.** Nie deklaruję potwierdzonej zgodności z konkretnym GPU ani wersją Resolve.

Porównano scalony kod z czterema oryginalnymi plikami Primera v0.6.0, commit `5b24e593390095968159e9ca55802feb0b35b64c`. Oryginały są umieszczone w oddzielnych przestrzeniach nazw w dołączonym stanowisku testowym. Referencja tworzy rzeczywisty pośredni obraz Primary → Hue → Split, a następnie podaje go do oryginalnego Skin. Wersja scalona otrzymuje tylko pierwotne wejście, więc sprawdzenie wykrywa m.in. błędne użycie surowych sąsiadów przy maskowaniu Skin.

- 101 konfiguracji: losowe parametry z pełnych zakresów suwaków, checkboxy, wszystkie 16 kombinacji Enable oraz ustawienia domyślne.
- Obrazy syntetyczne 257 × 17 i 3840 × 3. Szerokość 3840 pozwala sprawdzić rzeczywisty promień próbkowania UHD; nie jest to test pełnej klatki 4K ani wydajności.
- Kanały losowe od -0.2 do 1.2, ponadto czerń, szarość DI, biel i kolory podstawowe.
- Pierwsze 50 konfiguracji bez nakładek diagnostycznych; późniejsze obejmują również losowo aktywowane nakładki. Ostatnia konfiguracja sprawdza wartości domyślne i wszystkie moduły włączone.
- **2 417 910 porównań wartości kanałów. Maksymalna różnica bezwzględna: 0.** Próg akceptacji: 1e-6. W sprawdzonych próbkach brak NaN/Inf.
- Bypass All zwraca dokładnie wejściowe RGB dla każdego sprawdzanego piksela.
- Clang C++17 z AddressSanitizer i UndefinedBehaviorSanitizer: kompilacja i wykonanie zakończone powodzeniem, bez zgłoszeń sanitizerów.
- Statycznie: jedna funkcja wejściowa `transform`, unikalne nazwy funkcji i kontrolek, 49 float sliders i 17 checkboxów. Brak zewnętrznych include.
- Cztery oryginalne pliki DCTL są bajtowo zgodne z konkatenacją źródłowych fragmentów ich Makefile.

## Zakres zgodności

Zachowano algorytmy, zakresy i domyślne wartości oryginałów. Zmieniono nazwy symboli, dodano jawne przekazywanie parametrów przez strukturę, ustawiono transfer function na DI (indeks 5), dodano przełączniki sekcji i bypass. Skin pobiera centrum i sąsiadów po przeliczeniu wcześniejszych etapów. W etykietach usunięto emoji z nazw Squeeze. Licencję MIT zachowano w całości.

Test CPU emuluje część typów wektorowych i funkcji DCTL. Wykorzystuje ten sam kod matematyczny, ale nie parser DCTL ani implementacje Metal/CUDA/OpenCL. Potwierdza spójność scalenia z oryginałami, nie niezależną poprawność naukową oryginalnych algorytmów. Wynik 0 na CPU nie gwarantuje bitowej identyczności na GPU.

Nie sprawdzono: załadowania kontrolek w Resolve, kompilacji shaderów, renderu, szybkości odtwarzania, realnego materiału kamery, jakości skóry, HDR na monitorze, współpracy z zainstalowaną wersją SpektraFilm, animacji i zapisu parametrów w projekcie ani eksportu LUT. Udostępniony konektor informował o działającym Resolve Studio 21.1.1, lecz użyto go wyłącznie do dokumentacji API i zmian wersji. Nie modyfikowano projektu użytkownika ani folderu LUT.

## Powtórzenie testu CPU

Dołączony plik `validation/validate.cpp` zawiera scalony kod, oryginalne moduły oraz mały adapter CPU. Na systemie z Clang uruchom z folderu rozpakowanej paczki:

```sh
clang++ -std=c++17 -O1 -fsanitize=address,undefined validation/validate.cpp -o /tmp/primera-validate
/tmp/primera-validate
```

To utrwalony test tej dostawy, nie generator automatycznie wczytujący późniejsze edycje DCTL. Po zmianie pliku DCTL trzeba zaktualizować osadzoną wersję testową.

## Pierwszy test w Resolve

Po instalacji sprawdź brak DCTL Build Error. Na kontrolnym klipie DWG/Intermediate porównaj pojedynczy scalony węzeł z oryginalnymi czterema węzłami w tej samej kolejności i z Transfer Function = DaVinci Intermediate w Primary i Split. Przetestuj Bypass All, pojedyncze Enable, Soften = 0 / 0.25 / 1 oraz Separation = 0 / 0.5. Następnie sprawdź reprezentatywną twarz i barwy nasycone przy wyłączonych nakładkach oraz krótki eksport. Zachowaj tę samą przestrzeń przetwarzania węzłów i nie dodawaj między nimi transformacji koloru.
