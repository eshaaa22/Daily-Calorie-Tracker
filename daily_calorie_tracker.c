#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 64
#define MAX_DB 160
#define MAX_LOG 300
#define MAX_LINE 128

typedef struct {
    char name[MAX_NAME];
    char unit[MAX_NAME];
    int  kcal_per_unit;
    int  per100;
} Food;

typedef struct {
    char name[MAX_NAME];
    char unit[MAX_NAME];
    float qty;
    int kcal;
    int known;
} Logged;

Food  DB[MAX_DB]; int DB_N=0;
Logged LOG[MAX_LOG]; int LOG_N=0;

char  userName[64]; int userAge=0; char userGender='U';
float userWt=0.0f, userH_m=0.0f, userBMI=0.0f;
int   calorie_goal=2000, water_goal=8, water_intake=0;

void clearline(void){ int c; while((c=getchar())!='\n' && c!=EOF){} }
void readline(char* s, size_t n, const char* prompt){
    if(prompt && *prompt) printf("%s", prompt);
    if(fgets(s,(int)n,stdin)) s[strcspn(s,"\n")]=0; else s[0]=0;
}
int geti(const char* prompt){
    int x; printf("%s", prompt);
    while(scanf("%d",&x)!=1){ clearline(); printf("Invalid. %s", prompt); }
    clearline(); return x;
}
float getf(const char* prompt){
    float x; printf("%s", prompt);
    while(scanf("%f",&x)!=1){ clearline(); printf("Invalid. %s", prompt); }
    clearline(); return x;
}
int parse_feet_inches(const char* s,int* ft,int* in){
    int f=-1,i=-1; if(sscanf(s," %d'%d", &f,&i)==2 && f>=0 && i>=0 && i<12){*ft=f;*in=i;return 1;}
    return 0;
}
float feet_inches_to_m(int ft,int in){ return (ft*12+in)*0.0254f; }

float calc_bmi(float kg,float m){ return (m<=0)?0.0f:kg/(m*m); }
const char* bmi_cat(float b){
    if(b<18.5f) return "Underweight";
    if(b<25.0f) return "Normal";
    if(b<30.0f) return "Overweight";
    return "Obese";
}
int goal_from_bmi(float b){
    if(b<18.5f) return 2500;
    if(b<25.0f) return 2200;
    if(b<30.0f) return 1800;
    return 1600;
}

int iequal(const char* a,const char* b){
    while(*a && *b){
        char ca=(char)tolower((unsigned char)*a), cb=(char)tolower((unsigned char)*b);
        if(ca!=cb) return 0; ++a; ++b;
    } return *a==0 && *b==0;
}
int icontains(const char* hay,const char* needle){
    size_t n=strlen(needle); if(n==0) return 1;
    for(size_t i=0; hay[i]; ++i){
        size_t j=0; while(needle[j] && hay[i+j] &&
               tolower((unsigned char)hay[i+j])==tolower((unsigned char)needle[j])) j++;
        if(j==n) return 1;
    } return 0;
}

void add_food(const char* name,const char* unit,int kcal,int per100){
    if(DB_N>=MAX_DB) return;
    strncpy(DB[DB_N].name,name,MAX_NAME-1); DB[DB_N].name[MAX_NAME-1]=0;
    strncpy(DB[DB_N].unit,unit,MAX_NAME-1); DB[DB_N].unit[MAX_NAME-1]=0;
    DB[DB_N].kcal_per_unit=kcal; DB[DB_N].per100=per100; DB_N++;
}
void build_db(void){
    add_food("Rice","cup",200,0); add_food("Roti","piece",120,0);
    add_food("Chapati","piece",120,0); add_food("Paratha","piece",210,0);
    add_food("Bread","slice",60,0); add_food("Butter","tsp",35,0);
    add_food("Jam","tsp",20,0); add_food("Boiled Potato","medium",130,0);
    add_food("Boiled Veg","cup",80,0); add_food("Mixed Veg","cup",100,0);
    add_food("Salad","cup",80,0); add_food("Daal","cup",150,0);
    add_food("Oats Porridge","cup",150,0); add_food("Cornflakes + Milk","bowl",180,0);
    add_food("Khichdi","cup",200,0); add_food("Pulao","cup",250,0);
    add_food("Fried Rice","cup",300,0); add_food("Noodles","cup",220,0);
    add_food("Pasta","cup",200,0);
    add_food("Chicken Curry","serving",280,0); add_food("Beef Curry","serving",350,0);
    add_food("Fish Curry","serving",250,0); add_food("Shrimp Curry","serving",200,0);
    add_food("Paneer Curry","serving",280,0);
    add_food("Paneer (plain)","100 g",265,1);
    add_food("Dal Tadka","cup",180,0); add_food("Rajma Curry","cup",210,0);
    add_food("Chana Masala","cup",210,0); add_food("Tofu Stir-fry","cup",180,0);
    add_food("Omelet","serving",130,0); add_food("Boiled Egg","egg",78,0);
    add_food("Sandwich","piece",250,0); add_food("Club Sandwich","piece",400,0);
    add_food("Hot Dog","piece",300,0); add_food("Pancakes","medium",90,0);
    add_food("Waffles","medium",100,0); add_food("Honey Toast","slice",150,0);
    add_food("Croissant","piece",230,0); add_food("Muffin","piece",300,0);
    add_food("Donut","piece",250,0);
    add_food("Muri","cup",70,0); add_food("Fritters","piece",80,0);
    add_food("Samosa","piece",130,0); add_food("French Fries","cup",365,0);
    add_food("Spring Rolls","piece",100,0); add_food("Vegetable Pakora","piece",60,0);
    add_food("Vada Pav","piece",300,0); add_food("Pav Bhaji","plate",400,0);
    add_food("Sev Puri","plate",250,0); add_food("Pani Puri","plate",300,0);
    add_food("Kathi Roll","piece",350,0); add_food("Shawarma","piece",350,0);
    add_food("Kabab","skewer",150,0); add_food("Falafel","piece",57,0);
    add_food("Garlic Bread","slice",150,0); add_food("Pitha","piece",180,0);
    add_food("Pizza","slice",285,0); add_food("Burger","piece",500,0);
    add_food("Tacos","piece",170,0); add_food("Burrito","piece",430,0);
    add_food("Quesadilla","slice",300,0); add_food("Nachos with Cheese","cup",350,0);
    add_food("Spaghetti Bolognese","cup",350,0); add_food("Lasagna","slice",400,0);
    add_food("Risotto","cup",300,0); add_food("Sushi Roll (Veg)","6 pcs",200,0);
    add_food("Sushi Roll (Fish)","6 pcs",250,0);
    add_food("Ramen (instant)","bowl",380,0); add_food("Ramen (Tonkotsu)","bowl",500,0);
    add_food("Kimchi Fried Rice","cup",300,0); add_food("Chow Mein","cup",350,0);
    add_food("Pad Thai","cup",400,0); add_food("Pho","bowl",350,0);
    add_food("Bibimbap","bowl",490,0); add_food("Gimbap","roll",300,0);
    add_food("Thai Green Curry","cup",300,0); add_food("Thai Red Curry","cup",320,0);
    add_food("Cake","slice",400,0); add_food("Tart","slice",300,0);
    add_food("Kheer","bowl",220,0); add_food("Dessert (generic)","serving",250,0);
    add_food("Gulab Jamun","piece",150,0); add_food("Rasgulla","piece",120,0);
    add_food("Rasmalai","piece",150,0);
    add_food("Ice Cream (Vanilla)","scoop",137,0);
    add_food("Ice Cream (Chocolate)","scoop",150,0);
    add_food("Ice Cream (Strawberry)","scoop",140,0);
    add_food("Apple","medium",95,0); add_food("Banana","medium",110,0);
    add_food("Orange","medium",62,0); add_food("Mango","cup",150,0);
    add_food("Fruits (mixed)","cup",90,0);
    add_food("Milk","cup",150,0); add_food("Tea (milk+sugar)","cup",100,0);
    add_food("Green Tea","cup",2,0); add_food("Black Coffee","cup",2,0);
    add_food("Coffee (milk+sugar)","cup",110,0);
    add_food("Mango Lassi","glass",200,0); add_food("Banana Smoothie","glass",210,0);
    add_food("Strawberry Smoothie","glass",200,0); add_food("Chocolate Milkshake","glass",350,0);
    add_food("Iced Coffee","glass",100,0); add_food("Lemonade","glass",99,0);
    add_food("Herbal Tea","cup",5,0); add_food("Coconut Water","cup",46,0);
    add_food("Soft Drink","can",150,0); add_food("Orange Juice","cup",110,0);
    add_food("Energy Drink","can",160,0);
}

int find_food_index(const char* q){
    for(int i=0;i<DB_N;i++) if(iequal(DB[i].name,q)) return i;
    for(int i=0;i<DB_N;i++) if(icontains(DB[i].name,q)) return i;
    return -1;
}

void ask_qty_fixed(const Food* f, float* qty_out, int* kcal_out){
    printf("\nQuantity for %s (%d kcal per %s):\n", f->name, f->kcal_per_unit, f->unit);
    puts(" [1] 0.5");
    puts(" [2] 1");
    puts(" [3] 1.5");
    puts(" [4] 2");
    puts(" [5] 3");
    puts(" [6] Enter custom");
    int ch = geti("Choice: ");
    float q=1.0f;
    switch(ch){
        case 1:q=0.5f;break; case 2:q=1.0f;break; case 3:q=1.5f;break;
        case 4:q=2.0f;break; case 5:q=3.0f;break; case 6:q=getf("Enter quantity: "); if(q<0) q=0; break;
        default:q=1.0f;
    }
    *qty_out=q; *kcal_out=(int)(f->kcal_per_unit*q+0.5f);
}
void ask_qty_scaled(const Food* f, float* qty_out, int* kcal_out){
    printf("\n%s is per 100 g/ml (%d kcal/100).\n", f->name, f->kcal_per_unit);
    puts("Choose grams/ml:");
    printf(" [1] 50  (%d kcal)\n", (int)(f->kcal_per_unit*0.5f+0.5f));
    printf(" [2] 100 (%d kcal)\n", f->kcal_per_unit);
    printf(" [3] 150 (%d kcal)\n", (int)(f->kcal_per_unit*1.5f+0.5f));
    printf(" [4] 200 (%d kcal)\n", f->kcal_per_unit*2);
    puts(" [5] Enter grams/ml");
    int ch = geti("Choice: ");
    float g=100.0f;
    switch(ch){
        case 1:g=50;break; case 2:g=100;break; case 3:g=150;break; case 4:g=200;break;
        case 5:g=getf("Enter grams/ml: "); if(g<0) g=0; break;
        default:g=100;
    }
    *qty_out = g;
    *kcal_out = (int)(f->kcal_per_unit * (g/100.0f) + 0.5f);
}

void log_known(const Food* f){
    if(LOG_N>=MAX_LOG) return;
    float q=0.0f; int kc=0;
    if(f->per100) ask_qty_scaled(f,&q,&kc); else ask_qty_fixed(f,&q,&kc);
    strncpy(LOG[LOG_N].name,f->name,MAX_NAME-1); LOG[LOG_N].name[MAX_NAME-1]=0;
    strncpy(LOG[LOG_N].unit,f->unit,MAX_NAME-1); LOG[LOG_N].unit[MAX_NAME-1]=0;
    LOG[LOG_N].qty=q; LOG[LOG_N].kcal=kc; LOG[LOG_N].known=1; LOG_N++;
    printf("Logged: %s — %.2f %s — %d kcal\n", f->name, q, f->unit, kc);
}
void log_custom(const char* rawname){
    if(LOG_N>=MAX_LOG) return;
    puts("\nThis item is not in the saved list. We'll log it as custom.");
    puts("Quantity (servings):");
    puts(" [1] 0.5  [2] 1  [3] 1.5  [4] 2  [5] Enter…");
    int ch=geti("Choice: "); float q=1.0f;
    switch(ch){
        case 1:q=0.5f;break; case 2:q=1.0f;break; case 3:q=1.5f;break; case 4:q=2.0f;break;
        case 5:q=getf("Enter quantity: "); if(q<0) q=0; break; default:q=1.0f;
    }
    int per = geti("Enter calories per serving for this item: ");
    int kc = (int)(per*q+0.5f);
    strncpy(LOG[LOG_N].name,rawname,MAX_NAME-1); LOG[LOG_N].name[MAX_NAME-1]=0;
    strncpy(LOG[LOG_N].unit,"serving",MAX_NAME-1); LOG[LOG_N].unit[MAX_NAME-1]=0;
    LOG[LOG_N].qty=q; LOG[LOG_N].kcal=kc; LOG[LOG_N].known=0; LOG_N++;
    printf("Logged: %s — %.2f %s — %d kcal\n", rawname, q, "serving", kc);
}

int total_kcal(void){ int t=0; for(int i=0;i<LOG_N;i++) t+=LOG[i].kcal; return t; }

int main(void){
    build_db();

    puts("=== Smart Meal Planner & Calorie Tracker ===\n");

    readline(userName,sizeof(userName),"Enter Name: ");
    userAge = geti("Enter Age: ");
    printf("Enter Gender (M/F): "); scanf(" %c",&userGender); clearline();
    userWt  = getf("Enter Weight (kg): ");
    char hbuf[32]; int ft=0,in=0;
    readline(hbuf,sizeof(hbuf),"Enter your height like 5'8\": ");
    if(!parse_feet_inches(hbuf,&ft,&in)){ puts("Could not parse height; using 5'6\"."); ft=5; in=6; }
    userH_m = feet_inches_to_m(ft,in);
    printf("Recorded height: %d'%d\"\n", ft, in);

    puts("\nCalculating BMI...");
    userBMI = calc_bmi(userWt,userH_m);
    printf("BMI: %.2f\n", userBMI);
    printf("Category: %s\n\n", bmi_cat(userBMI));
    calorie_goal = goal_from_bmi(userBMI);
    water_goal   = 8;

    puts("According to your BMI:");
    printf(" - Calorie goal: %d kcal\n", calorie_goal);
    printf(" - Water goal  : %d glasses\n", water_goal);

    puts("\nEnter meal/food names (one per line).");
    puts("Press ENTER on a blank line to finish.\n");
    char namebuf[MAX_NAME];
    char queued[MAX_LOG][MAX_NAME]; int QN=0;

    while(QN < MAX_LOG){
        printf("%d) ", QN+1);
        readline(namebuf, sizeof(namebuf), "");
        if(strlen(namebuf)==0) break;
        strncpy(queued[QN], namebuf, MAX_NAME-1);
        queued[QN][MAX_NAME-1]=0;
        QN++;
    }

    for(int i=0;i<QN;i++){
        int idx = find_food_index(queued[i]);
        if(idx>=0) log_known(&DB[idx]);
        else       log_custom(queued[i]);
    }

    puts("\n===== Today's Meals =====");
    if(LOG_N==0) puts("(none)");
    for(int i=0;i<LOG_N;i++){
        printf("%2d) %-16s — %.2f %-8s — %d kcal\n",
               i+1, LOG[i].name, LOG[i].qty, LOG[i].unit, LOG[i].kcal);
    }
    int total = total_kcal();
    printf("---------------------------------------------\n");
    printf("Total calories: %d / %d  -> %s\n",
           total, calorie_goal, (total>calorie_goal?"❗ Over goal":"✅ Within goal"));

    water_intake = geti("\nWater intake so far (glasses): ");
    if(water_intake < water_goal)
        printf("Water goal: %d -> You need %d more glasses today.\n",
               water_goal, water_goal - water_intake);
    else
        printf("Water goal met! %d / %d\n", water_intake, water_goal);

    puts("\n=== Status Now ===");
    printf("Calories: %d / %d  -> %s\n",
           total, calorie_goal, (total>calorie_goal?"Over target":"On/under target"));
    printf("Water   : %d / %d\n", water_intake, water_goal);

    puts("\nNotes:");
    if(total <= calorie_goal) puts(" - You're within your calorie goal. Keep it balanced.");
    else                      puts(" - You're over the calorie goal; keep evening lighter.");
    if(water_intake < water_goal) puts(" - Add more water across the day.");
    else                          puts(" - Hydration goal achieved. Great job!");

    puts("\nGoodbye! Stay healthy.");
    return 0;
}
