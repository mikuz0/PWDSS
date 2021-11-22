# PWDSS
<p><img src="http://www.mkuznecov.ru/sites/default/files/virtualbox_win7_qt_20_06_2020_13_15_50.png" alt="" width="300" vspace="10" hspace="10" align="left">Как показывает анализ интернета, в России есть целая серия предприятий, производящих солемеры, принцип действия которых основан на <a href="https://ru.wikipedia.org/wiki/%D0%9A%D0%BE%D0%BD%D0%B4%D1%83%D0%BA%D1%82%D0%BE%D0%BC%D0%B5%D1%82%D1%80%D0%B8%D1%8F" target="_blank" rel="noopener">кондуктометрии</a>.&nbsp;Что делает возможным (нет комерческой и гос. тайны) и целесообразным (самореклама и помощь коллегам)&nbsp; публикацию данного материала.</p><p><strong>Постановка задачи:</strong></p><p><strong>Дано:</strong> есть разработанный другими людьми солемер с написанным програмным обеспечением. Математика измерений общеизвестна и описана в литературе, хотя и несколько объемна. Есть стенд с физическими растворами для моделирования всей физики.</p><p><strong>Проблема:</strong> процесс настройки связан с рекурсивной,&nbsp; интуитивной настройкой, путем ввода подстроечных коэффициентов и последующей проверкой на жидкости с известной температурой и соленостью. ЭТО ОЧЕНЬ ДОЛГО. До 3-4 рабочих смен на комплект.</p><p><!--break--></p><p><strong>Задача:</strong> Найти решение, которое позволяет значительно снизить трудоемкость процесса.</p><p><strong>Решение:</strong> Предложение достаточно очевидно: написать программу, которая рассчитывает настроечные коэффициенты по снятым параметрам физической среды. Параметры среды снимаются на настраиваемом приборе и образцовыми приборами.</p><p>Анализ ситуации с солемером показал, что для настройки достаточно снять данные в двух температурных точках и двух точках по солености.</p><p>Далее была составлена система уравнений и решена при помощи системы символьной алгебры <a href="http://maxima.sourceforge.net/ru/" target="_blank" rel="noopener">Maxima</a>.</p><p>Таким образом были получены формулы для расчета.</p><p>Графический интерфейс пользователя был оформлен&nbsp; с использованием <a href="https://ru.wikipedia.org/wiki/Qt" target="_blank" rel="noopener">фреймворка Qt</a>. Хотя первоначальная, тестовая версия была написано как html&nbsp; страница с вложенным JavaScript, что делало решение более кросплатформенным, но не защищенным от вмешательства и удаления копирайта.</p><p>Внешний вид программы в среде Windows 7 приведен в анонсе материала.</p><p>Программа содержит несколько вкладок, которые позволяют настраивать как комплекс,&nbsp; так и вторичный преобразователь отдельно.</p><p><a href="http://www.mkuznecov.ru/kontakty.html" target="_blank" rel="noopener">Мои контакты тут</a>.</p>
