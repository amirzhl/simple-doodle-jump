# 🕹️ Simple Doodle Jump

یک بازسازی تمیز و کامل از بازی کلاسیک **Doodle Jump** با زبان **C++17** و کتابخانه‌ی گرافیکی **SFML 2.6**.  
این پروژه به‌عنوان تمرین درس **برنامه‌نویسی پیشرفته (Advanced Programming)** و با تمرکز جدی روی **شی‌گرایی (OOP)**، **چندریختی (Polymorphism)** و اصول **Clean Code** نوشته شده است.

> بازیکن به‌صورت خودکار می‌پرد؛ شما فقط با کلیدهای **← / →** او را چپ و راست هدایت می‌کنید و تا جای ممکن بالا می‌روید. افتادن به پایین صفحه = پایان بازی.

---

## ✨ ویژگی‌ها

- 🎮 گیم‌پلی کامل Doodle Jump: پرش خودکار، دوربین بی‌نهایت رو به بالا، و امتیاز بر اساس ارتفاع.
- 🧱 سه نوع سکو با رفتار چندریخت (polymorphic):
  - **Normal** — سکوی استاندارد (گاهی همراه با **فنر/Spring** برای پرش خیلی بلند).
  - **Moving** — سکوی متحرک که افقی حرکت می‌کند و از دیوارها برمی‌گردد.
  - **Breakable** — سکوی شکستنی که بعد از یک بار فرود می‌شکند و می‌افتد.
- 🌀 **فنر دو-فریمی** با `setTextureRect` (حالت باز/فشرده).
- 🗺️ تولید تصادفی وزن‌دار سکوها با **تضمین دسترسی‌پذیری**: بازی همیشه قابل‌ادامه است (هیچ پرشی غیرممکن نمی‌شود).
- ♻️ مدیریت حافظه‌ی امن: سکوهایی که از صفحه خارج می‌شوند حذف و سکوهای جدید ساخته می‌شوند (بدون نشتی حافظه).
- 🏆 ذخیره‌ی دائمی **بالاترین امتیاز** در فایل `highscore.txt`.
- 🖥️ سه صفحه‌ی مجزا: **منو**، **بازی**، **پایان بازی** (State Pattern).
- 🧩 معماری تمیز، ماژولار و مستند، آماده‌ی توسعه‌ی بلندمدت.

---

## 🎯 کنترل‌ها

| ورودی | عملکرد |
|-------|--------|
| `←` فلش چپ | حرکت به چپ |
| `→` فلش راست | حرکت به راست |
| کلیک موس | فشردن دکمه‌ها (Start / Restart / Main Menu) |
| بستن پنجره | خروج از بازی |

پرش خودکار است؛ هر بار که روی یک سکو فرود می‌آیی دوباره پرتاب می‌شوی.

---

## 🧱 معماری و طراحی

پروژه کاملاً شی‌گرا و بر پایه‌ی چندریختی از طریق **اشاره‌گر به کلاس پایه + متدهای virtual** ساخته شده است. رفتار متفاوت هر سکو با **dynamic binding** حل می‌شود.

### سلسله‌مراتب کلاس‌ها

```
Entity  (abstract: update / draw / getBounds)
├── Player
├── Spring
└── Platform  (abstract: onLand / isSupportive / supportVelocity)
    ├── NormalPlatform     (می‌تواند یک Spring داشته باشد)
    ├── MovingPlatform
    └── BreakablePlatform

State  (abstract: handleEvent / update / render)
├── MenuState
├── PlayState
└── GameOverState
```

### تصمیم‌های کلیدی طراحی

- **تک‌مسئولیتی (SRP):** هر کلاس فقط یک کار دارد — `Player` فقط حرکت/فیزیک خودش، `CollisionSystem` فقط برخوردها، `PlatformManager` فقط تولید/حذف سکوها، و `Game` فقط حلقه‌ی اصلی و منابع.
- **برخورد متمرکز:** همه‌ی سکوها از یک منطق برخورد واحد عبور می‌کنند؛ *پاسخِ* متفاوت با `Platform::onLand()` به‌صورت چندریخت واگذار می‌شود (بدون تکرار کد).
- **`std::vector<Platform*>`** برای نگهداری سکوها (نه `vector<Platform>`) تا چندریختی حفظ شود.
- **فیزیک مستقل از نرخ فریم:** همه‌ی حرکت‌ها در `dt` ضرب می‌شوند و `dt` برای پایداری کلمپ می‌شود.
- **بدون Magic Number:** تمام مقادیر قابل‌تنظیم در `include/core/Constants.hpp` (namespace `cfg`) متمرکز شده‌اند.
- **دوربین:** یک `sf::View` عمودی که فقط رو به بالا حرکت می‌کند؛ بازیکن واقعاً در فضای دنیا بالا می‌رود و دنیا زیرش اسکرول می‌شود.

### تضمین دسترسی‌پذیری (Accessibility)

حداکثر فاصله‌ی عمودی بین دو سکو `MAX_GAP = 95px` است و `2 × 95 = 190px` از حداکثر ارتفاع پرش معمولی (`≈ 225px`) کمتر است. همچنین هیچ‌گاه دو سکوی شکستنی پشت‌سرهم قرار نمی‌گیرند (نوع سکوی قبلی با `dynamic_cast` تشخیص داده می‌شود). نتیجه: بازی همواره قابل‌ادامه است.

---

## ✅ تطبیق با الزامات پروژه

این فاز دقیقاً طبق چک‌لیست الزامات پیاده‌سازی شده است:

| الزام | وضعیت | جزئیات در کد |
|------|--------|-------------|
| منطق کامل در کلاس‌ها (OOP) | ✅ | `main` فقط `Game` را می‌سازد و اجرا می‌کند |
| سطح دسترسی اعضا (`private`/`protected`) | ✅ | هیچ عضو داده‌ی `public` وجود ندارد |
| `main` کوتاه و غیررویه‌ای | ✅ | `src/main.cpp` |
| **Template معنادار** | ✅ | `ResourceManager<T>` با `sf::Texture` و `sf::Font` — بارگذاری/کش یک‌باره‌ی دارایی‌ها |
| **`static_cast`** برای تبدیل عددی | ✅ | در محاسبات فیزیک، چیدمان و امتیاز |
| **`dynamic_cast`** برای تشخیص نوع سکو | ✅ | `PlatformManager::lastPlatformIsBreakable()` |
| پرهیز از cast سبک C | ✅ | هیچ cast سبک C در کد نیست |
| مدیریت حافظه (بدون leak) | ✅ | هر `new` یک `delete` دارد + `unique_ptr` |
| ساختار چندفایلی (`.hpp`/`.cpp`) | ✅ | تفکیک کامل `include/` و `src/` |
| طراحی توسعه‌پذیر (دید بلندمدت) | ✅ | افزودن ویژگی = افزودن یک کلاس فرزند |
| Clean Code | ✅ | نام‌گذاری استاندارد، توابع کوچک، بدون Magic Number، DRY |

### قالب معنادار (Template)
`ResourceManager<Resource>` یک کش عمومی برای هر دارایی‌ای است که متد `loadFromFile` دارد. هر فایل فقط یک‌بار از دیسک خوانده می‌شود. اکنون با `sf::Texture` و `sf::Font` استفاده می‌شود و در فاز بعد مستقیماً برای `sf::SoundBuffer` قابل‌استفاده است.

### تبدیل نوع (Type Casting)
- `static_cast` — برای تمام تبدیل‌های عددی (مانند `int` به `float`).
- `dynamic_cast` — در `PlatformManager::lastPlatformIsBreakable()` برای تشخیص اینکه آخرین سکوی ساخته‌شده از نوع `BreakablePlatform` است یا نه (اجرای قانون «دو سکوی شکستنی پشت‌سرهم نباشد»).

### مدیریت حافظه
- سکوها با `new` ساخته و در `PlatformManager::removeExpired` (هنگام خروج از صفحه) و `clear()` (در مخرب) با `delete` آزاد می‌شوند.
- فنرها، State فعال، و منابع با `std::unique_ptr` مدیریت می‌شوند (آزادسازی خودکار — RAII).

---

## 📁 ساختار پروژه

کد بر اساس **کارکرد** در زیرپوشه‌های آینه‌ای `include/` و `src/` سازمان‌دهی شده است:

```
simple-doodle-jump/
├── assets/                     # تصاویر بازی (پس‌زمینه، دودل، سکوها، فنر، دکمه‌ها)
├── fonts/                      # فونت (ariblk.ttf)
├── include/
│   ├── core/       (Constants, Game, ResourceManager<T>)
│   ├── entities/   (Entity, Player, Spring, platforms/*)
│   ├── world/      (CollisionSystem, PlatformManager)
│   ├── ui/         (Button, Layout)
│   ├── states/     (State, StateId, Menu/Play/GameOver)
│   └── utils/      (Random)
├── src/                        # پیاده‌سازی‌ها (آینه‌ی include/)
├── makefile                    # ساخت چندسکویی (Windows / Linux / macOS)
├── highscore.txt               # بالاترین امتیاز ذخیره‌شده
├── .gitignore
└── README.md
```

> بدنه‌ی متدها همیشه در فایل `.cpp` نوشته شده تا فایل‌های هدر تمیز بمانند.

---

## 🛠️ پیش‌نیازها

1. کامپایلر **g++** با پشتیبانی **C++17**
2. ابزار **make** (روی ویندوز: `mingw32-make`)
3. کتابخانه‌ی **SFML 2.5 یا 2.6** (ماژول‌های `graphics`, `window`, `system`)

> ⚠️ کد برای **SFML 2.x** نوشته شده است. با **SFML 3** به‌طور مستقیم کامپایل نمی‌شود (API نسخه ۳ متفاوت است).

---

## ▶️ ساخت و اجرا

### لینوکس (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install g++ make libsfml-dev
make
make run
```

### macOS (Homebrew)
```bash
brew install sfml@2
# Apple Silicon:
make SFML_INC=-I/opt/homebrew/opt/sfml@2/include SFML_LIB=-L/opt/homebrew/opt/sfml@2/lib
./simple-doodle-jump
```

### ویندوز (MinGW-w64 + Git Bash)
1. کامپایلر **GCC** (WinLibs 64-bit) و **SFML 2.6.1 هم‌نسخه با کامپایلر** (GCC MinGW SEH 64-bit) را نصب کن (نسخه‌ی کامپایلر و SFML باید یکی باشد).
2. در **Git Bash**، از ریشه‌ی پروژه (مسیر SFML را مطابق نصب خودت تنظیم کن):
```bash
mingw32-make SFML_INC=-IC:/SFML/include SFML_LIB=-LC:/SFML/lib
```
> اگر SFML در مسیری با فاصله (مانند `C:\Program Files\SFML-2.6.1`) نصب شده، از مسیر کوتاه استفاده کن تا فاصله مشکل ایجاد نکند:
> `mingw32-make "SFML_INC=-IC:/PROGRA~1/SFML-2.6.1/include" "SFML_LIB=-LC:/PROGRA~1/SFML-2.6.1/lib"`
3. فایل‌های `*.dll` داخل `C:\SFML\bin` را کنار `simple-doodle-jump.exe` کپی کن (یا `C:\SFML\bin` را به PATH اضافه کن).
4. اجرا از ریشه‌ی پروژه:
```bash
./simple-doodle-jump.exe
```

### دستورهای makefile
| دستور | کار |
|-------|-----|
| `make` | ساخت بازی |
| `make run` | ساخت و اجرا |
| `make clean` | پاک‌کردن خروجی‌های ساخت |

> 📌 بازی را حتماً از **ریشه‌ی پروژه** اجرا کن؛ مسیر `assets/`، `fonts/` و `highscore.txt` نسبی هستند.

---

## 🧼 استاندارد کد و نام‌گذاری

این پروژه از یک گایدلاین نام‌گذاری واحد پیروی می‌کند که در ابتدای هر فایل هم اعلام شده است:

| دسته | قاعده | نمونه |
|------|-------|-------|
| کلاس‌ها / تایپ‌ها | `PascalCase` | `PlatformManager` |
| توابع / متدها | `camelCase` | `handleInput` |
| متغیرها و پارامترها | `lower_snake_case` | `world_top` |
| متغیرهای عضو | `lower_snake_case_` (زیرخط انتهایی) | `high_score_` |
| ثابت‌ها | `UPPER_SNAKE_CASE` | `MOVE_SPEED` |

اصول رعایت‌شده: توابع کوچک و تک‌منظوره، `main` کوتاه، عدم استفاده از Magic Number، اصل DRY، کامنت‌های «چرایی» (نه بدیهی)، توابع `const` برای متدهای فقط‌خواندنی، و پیاده‌سازی متدها بیرون از تعریف کلاس.

---

## 📤 قراردادن روی گیت‌هاب

ابتدا در github.com یک ریپازیتوری خالی بساز (بدون README)، سپس از ریشه‌ی پروژه:

```bash
git init
git add .
git commit -m "Simple Doodle Jump - C++/SFML final project"
git branch -M main
git remote add origin https://github.com/<username>/simple-doodle-jump.git
git push -u origin main
```

فایل `.gitignore` از پیش فایل‌های ساخت (`build/`, `*.o`, `*.exe`) را کنار می‌گذارد.

---

## 👤 نویسنده

Amir — تمرین درس برنامه‌نویسی پیشرفته (C++).

<div align="center">

ساخته‌شده با ❤️ و C++17 — *Computer Assignment (project)*

</div>

