# Enum

`Enum8` 或者 `Enum16`。 一组有限的字符串值，比 `String` 类型的存储更加有效。

示例:

```text
Enum8('hello' = 1, 'world' = 2)
```

- 一个类型可以表示两个值: 'hello' and 'world'。

`Enum8` 类型的每个值范围是 `-128 ... 127`，`Enum16` 类型的每个值范围是 `-32768 ... 32767`。所有的字符串或者数字都必须是不一样的。允许存在空字符串。如果某个 Enum 类型被指定了（在表定义的时候），数字可以是任意顺序。然而，顺序并不重要。
（译者注：如 `Enum8('he o' = 3, 'wld' = 1)` 也是合法的）

在内存中，Enum 类型当做 `Int8` or `Int16` 对应的数值来存储。
当以文本方式读取的时候，ClickHouse 将值解析成字符串然后去 Enum 值的集合中搜索对应字符串。如果没有找到，会抛出异常。当读取文本格式的时候，会根据读取到的字符串去找对应的数值。如果没有找到，会抛出异常。

当以文本方式写入的时候，ClickHouse 将值解析成字符串写入。如果  column 数据包含垃圾（不是从有用集合含有的数值），会抛弃异常。Enum 类型以二进制读取和写入的方式与 Int8 和 Int16 类型一样的。
隐式默认值是对应类型的最小值。

在 `ORDER BY`, `GROUP BY`, `IN`, `DISTINCT` 中，Enums 和对应数值是一样的工作方式。比如， ORDER BY 会将它们按数值排序。对 Enums 类型使用相同和比较操作符都与操作它们隐含的数值是一样的。

Enum 值不能和数值比较大小。Enums 可以和一个常量字符串比较大小。如果字符串不是一个可用的 Enum 值，会抛出异常。可以使用 IN 运算符来判断一个 Enum 是否存在于某个 Enum 集合中，其中集合中的 Enum 需要用字符串表示。

大部分数字运算和字符串运算都没有给 Enum 类型定义，比如，Enum 类型不能和一个数相加，或进行字符串连接的操作，但是可以通过 toString 方法返回它对应的字符串。

Enum 值使用 `toT` 函数可以转换成数值类型，其中 T 是一个数值类型。若 `T` 恰好对应 Enum 的底层数值类型，这个转换是零消耗的。

Enum 类型可以被 `ALTER` 无成本地修改对应集合的值。可以通过 `ALTER` 操作来增加或删除 Enum 的成员（只要表没有用到该值，删除都是安全的）。作为安全保障，改变之前使用过的 Enum 成员将抛出异常。

通过 `ALTER` 操作，可以将 `Enum8` 转成 `Enum16`，反之亦然，就像 `Int8` 转 `Int16`一样。

[来源文章](https://clickhouse.yandex/docs/en/data_types/enum/) <!--hide-->
