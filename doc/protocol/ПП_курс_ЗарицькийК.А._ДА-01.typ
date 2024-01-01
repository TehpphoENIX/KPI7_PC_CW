#import "template.typ":*
#import "@preview/codelst:1.0.0": sourcecode

#let codelst-sourcecode = sourcecode
#let sourcecode = codelst-sourcecode.with(
frame: block.with(
fill: none,
stroke: none,
radius: 2pt,
inset: (x: 10pt, y: 5pt)
)
)

#show: project.with(
  title: [
    Курсова Робота\
    з дисципліни\
    Паралельні обчислення
  ],
  author: "Зарицький Кирило Андрійович"
)

#outline(
  title: [Зміст],
  depth: 3,
)

#set heading(numbering: "1.1.1")

#show heading.where(
  level: 1
): it => [
  #pagebreak(weak: true)
  #it.body
]

#pagebreak()
= Вступ

#[



]

= Теоретичні Відомості

#[



]

= Модель Системи

#[



]

= Опис Реалізації

#[

  В цьому розділі буде розглянуто реалізацію моделі системи. Далі   буде розглянуто структуру проекту, опис його модулів та   синхронізації в цих модулях. Також в кінці розділу є інструкція з   інсталювання.

  Загалом проект був розроблений на мові програмування C++ для Unix   подібних систем (в першу чергу Linux).

  == Структура Проекту

  #[
    Побудова, інсталювання та тестування проекту виконано за допомогою  системи CMake. Її конфігураційні файли розташовані в кореневій   директорії проекту та директорії tests.

    В директоріях include та src розташовані файли в яких описані     класи проекту (в "include\/\*.h" -- опис класів, а в "src\/\*.  cpp"  -- реалізації). Така місткова система є стандартною   практикою в  написанні C++ коду.

    В директорії exec розташованні функції main відповідних   виконавчих  файлів. В проекті збирається 4 програми:
      + CWServer -- програма, що збирає індекс та потім відкриває     інтернет сокет для надання сервісу.
      + CWClient -- програма-клієнт, що отримує сервіс.
      + CWMultiClient -- програма, що робить стрес-експеримент    запускаючи N потоків, що звертаються до сервісу одночасно.
      + tests/CommonCxxTests -- тести системи. В проекті є 5  модульних тестів, що можуть бути виконані в цій програмі   (зауважимо, що тести не виводять дані, тому для перевірки   корректності їх виконання слід дивитись на код, що вони   повертають (0 = успіх)):
        + invertedindex_write_multithread_test -- тест на   багатопоточний запис в індекс.
        + invertedindex_write_singlethread_test -- тест на  однопоточний запис в індекс.
        + threadpool_completion_test -- тест на завершення виконання  всіх завдань в пулі потоків.
        + threadpool_subscriber_test -- тест на перевірку підписки  на завершення роботи пулу потоків.
        + threadpool_thread_count_test -- тест на запуск правильної   кількості робочих потоків.

    В директоріях test_data та eval_data розташовані дані для   побудови індексу. Перший -- невеликий набір даних для наочної   перевірки роботи програми. Другий -- набір даних за варіантом   курсової роботи. Зауважимо, що користувач може сам надавати   програмі дані.

    В директорії doc розташована документація проекту. В директорії models -- діаграми моделі системи.
  ]

  == Опис Модулів

  #[
    Прогама має в собі 2 класи, 2 незалежні функції та головну функцію.

    === InvertedIndex
    defined in \<invertedindex.h>
    ```cpp
    class InvertedIndex;
    ```
    Реалізація інвертованого індексу. В середині він з себе представляє індивідуально реалізовану hash невпорядкований набір (hast-таблиця) з пар слово -- набір назв документів в яких воно з'являється(завдяки реалізації набір документів відсортований).  

    ==== (constructor)
    ```cpp
    InvertedIndex(const std::size_t initialSize = defaultInitialSize, const float loadFactor = defaultLoadFactor);
    ```
    Будує клас з заданим початковим розміром та заданим loadFactor.
    - *initialSize* -- початковий розмір масиву backets hash-таблиці.
    - *loadFactor* -- loadFactor hash-таблиці. Якщо цей вираз вірний:\
    $"elements in array"/"array size" > "loadFactor"$\
    то розмір масиву збільшується в двічі.
    
    ==== insert
    ```cpp
    void insert(const std::string token, const std::string document);
    ```
    Додає пару *token*-docu*ment до індексу. Цей метод блокуючий! Виконання цієї операції неможливе після виклику *finish*.
    - *token* -- слово, з яким асоційовується документ.
    - *document* -- назва документу, що буде асоційований зі словом.
    ==== insertBatch
    ```cpp
    void insertBatch(const std::vector<std::pair<std::string,std::string>>& pairs);
    ```
    Додає серію пар *token*-*document* до індексу. Цей метод блокуючий! Виконання цієї операції неможливе після виклику *finish*.
    - *pairs* -- масив пар слово-документ, що будуть додані до масиву.
    ==== find
    ```cpp
    bool find(const std::string token);
    ```
    Шукає слово *token* в масиві. Виконання цієї операції неможливе перед викликом *finish*.
    - *token* -- слово, що треба знайти.
    - *Повертає* -- _true_, якщо слово є в індексі та _false_ в іншому випадку.
    ==== read
    ```cpp
    const std::set<std::string>& read(const std::string token);
    ```
    Зчитує документи в яких зустрічається слово *token*. Виконання цієї операції неможливе перед викликом *finish*.
    - *token* -- слово, що треба знайти.
    - *Повертає* -- набір документів, в яких слово присутнє в індексі; кидає _std::exception_ в іншому випадку.
    ==== finish
    ```cpp
    void finish();
    ```
    Завершує редагування класу та відкриває його для читання.

    === ThreadPool
    defined in \<threadpool.h>
    ```cpp
    class ThreadPool;
    ```
    Реалізація пулу потоків. В середині використовує пріоритетну чергу для завдань.
    ==== Task
    ```cpp
    typedef std::pair<int, std::function<const int()>> Task;  
    ```
    Тип "завдання" є важливим для додавання завдань в чергу. Представляє з себе пару з цілого числа та функції, що бере 0 аргументів та повертає ціле число.
    ==== (constructor)
    ```cpp
    ThreadPool(unsigned int N, bool exitImmediatlyOnTerminate = false);
	  ThreadPool(const ThreadPool&) = delete;
	  ThreadPool(ThreadPool&& other) = delete;
	  ThreadPool& operator=(ThreadPool& rhs) = delete;
	  ThreadPool& operator=(ThreadPool&& rhs) = delete;
    ```
    Будує клас з заданою кількістю потоків та стандартною поведінкою деструктора. Зауважимо, що copy та move семантика для класу заборонені (видалені).
    - *N* -- кількість потоків пулу потоків.
    - *exitImmediatlyOnTerminate* -- поведінка виходу. В залежності від обраного значення буде змінена поведінка деструктора.
    ==== (destructor)
    ```cpp
	  ~ThreadPool();
    ```
    Зупиняє роботу класу. В залежності від значення флагу *exitImmediatlyOnTerminate*, заданому при створенні, буде викликано або terminateIm() для _true_, або terminate навпаки.
    ==== terminate/terminateIm
    ```cpp
	  void terminate();
	  void terminateIm();
    ```
    Зупиняє роботу класу. Всі потоки або відокремлюються при *terminateIm* або приєднуються до потоку виклику при *terminate*.
    ==== pause/unpause/pauseToggle
    ```cpp
	  void pause();
	  void unpause();
	  void pauseToggle();
    ```
    Призупиняє виконання наступних завдань. Ці функції не не призупиняють самі потоки, а лише не дають їм взяти нові завдання з черги.
    ==== addTask
    ```cpp
	  void addTask(Task task);
    ```
    Додає завдання в чергу.
    - *task* -- завдання, що буде додано.
    ==== removeTask
    ```cpp
	  void removeTask();
    ```
    Видаляє останнє завдання з черги.
    ==== currentQueueSize
    ```cpp
    unsigned int currentQueueSize();
    ```
    Повертає поточну довжину черги.
    ==== currentThreadStatus
    ```cpp
	  std::unordered_map<unsigned short, ThreadPool::threadStatusEnum> currentThreadStatus();
    ```
    Повертає поточний статус всіх потоків.
    ==== toString
    ```cpp
	  static const char* toString(ThreadPool::threadStatusEnum v)
    ```
    Повертає строку, що пояснює стан потоку.
    *v* -- статус потоку, який треба пояснити.
    ==== numberOfThreds
    ```cpp
    unsigned int numberOfThreds()
    ```
    Повертає кількість потоків в класі.
    ==== avgWaitTime
    ```cpp
	  double avgWaitTime();
    ```
    Повертає середній час очікування на завдання.
    ==== avgWaitTimeReset
    ```cpp
	  void avgWaitTimeReset();
    ```
    Скидує дані про середній час очікування.
    ==== avgQueueSize
    ```cpp
	  double avgQueueSize();
    ```
    Повертає середню довжину черги.
    ==== avgQueueSizeReset
    ```cpp
	  void avgQueueSizeReset();
    ```
    Скидує дані про середню довжину черги.
    ==== avgTaskCompletionTime
    ```cpp
	  double avgTaskCompletionTime();
    ```
    Повертає середній час виконання завдання.
    ==== avgTaskCompletionTimeReset
    ```cpp
	  void avgTaskCompletionTimeReset();
    ```
    Скидає дані про середній час виконання.
    ==== subscribeOnFinish
    ```cpp
	  std::pair<std::set<std::function<void ()>>::iterator, bool> subscribeOnFinish (std::function<void()> callback);
    ```
    Підписується на подію завершення виконання. Ця подія відбувається коли всі потоки очікують елементу черги та черга пуста.
    - *callback* -- функція, що буде викликана за настання події.
    - *Повертає* -- пару з іттератору, що вказує на додану функцію та флагу, що показує чи був елемент доданий до списку підписників.
    ==== unsubscribeOnFinish
    ```cpp
	  void unsubscribeOnFinish(std::set<std::function<void ()>>::iterator itterator);
    ```
    Відписатись від події завершення виконання.
    - *itterator* -- іттератор, що вказує на елемент, що був доданий.

    === HandleRegularFile
    defined in "exec/CWServer.cpp"
    ```cpp
    const int HandleRegularFile(const std::filesystem::path filePath, InvertedIndex& invIn);
    ```
    Процедура, що переглядає всі слова в файлі та будує їх набір. З цього набору формуються пари слово-документ, що потім всі разом додаються до індексу
    - *filePath* -- шлях до файлу.
    - *invertedIndex* -- індекс, до якого слід додати слова з файлу.

    === HandleFile
    defined in "exec/CWServer.cpp"
    ```cpp
    const int HandleFile(const std::filesystem::path filePath, ThreadPool& threadPool, InvertedIndex& invIn);
    ```
    Процедура, що оброблює файли. Якщо він директорія, то процедура переглядає його вміст додає рекурсивні виклики над цими файлами до пулу потоків. Якщо він звичайний файл, то додає процедуру HandleRegularFile над цим файлом до пулу потоків.
    - *filePath* -- шлях до файлу.
    - *threadPool* -- пул потоків, до якого слід додавати процедури.
    - *invertedIndex* -- індекс, що передається функції HandleRegularFile.
  ]

  == Опис синхронізації

  #[

  ]

  == Керівництво з Встановлення Програми

  #[
    
  ]
  
]

= Виконання програми

#[

]

= Висновки

#[

]


