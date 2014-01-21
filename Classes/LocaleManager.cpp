#include "LocaleManager.h"

namespace EMCore {

   const string LocaleManager::implemented_langs[] = {
      "lang/russian.xml" // Russian
   };

   // Helper dictionary class
   class CC_DLL Dictionary {
   public:

      Dictionary() {}
      virtual ~Dictionary() {}

      static Dictionary* create(const string& path) {

         auto fab = new Dictionary();

         auto full_path = FileUtils::getInstance()->fullPathForFilename(path);

         if (fab->initWithData(full_path)) {
            return fab;
         }

         delete fab;
         return nullptr;
      }

      bool initWithData(const string& path) {

         EM_LOG_DEBUG("Reading dictionary " + path);

         tinyxml2::XMLDocument doc;
         doc.LoadFile(path.c_str());

         if (doc.Error()) {
            EM_LOG_ERROR("Cant parse file, Txml says: \n " + doc.GetErrorStr1() + "\n" + doc.GetErrorStr2()); return false;
         }

         auto root = doc.FirstChildElement("language");
         if (root == nullptr) { 
            EM_LOG_ERROR("No `language` element"); return false;
         }

         auto n = root->Attribute("name");
         if (n == nullptr) {
            EM_LOG_ERROR("No `name` attribute"); return false;
         }
         name = n;

         auto on = root->Attribute("original_name");
         if (on == nullptr) {
            EM_LOG_ERROR("No `original_name` attribute"); return false;
         }
         original_name = on;

         unsigned cnt = 42;
         root->QueryUnsignedAttribute("strings", &cnt);
         data.reserve(cnt);

         auto s = root->FirstChildElement("s");
         while (s)
         {
            auto key = s->Attribute("k");
            if (key == nullptr) {
               EM_LOG_WARNING("No `k` attribute in string, skipping");
               s = s->NextSiblingElement("s");
               continue;
            }

            auto value = s->GetText();
            if (value == nullptr) {
               EM_LOG_WARNING("No value for '" + key + "', skipping");
               s = s->NextSiblingElement("s");
               continue;
            }

            data.emplace(key, value);
            s = s->NextSiblingElement("s");
         }


         return true;
      }

      const string& GetName() { return name; }
      const string& GetOriginalName() { return original_name; }
      virtual const string& Lookup(const string& s) {
         auto it = data.find(s);
         if (it != end(data)) {
            return it->second;
         }
         EM_LOG_WARNING("Locale: `"  + s + "` has no translation in " + name );
         return s;
      }
      virtual const char* Lookup(const char* s) {
         auto it = data.find(s);
         if (it != end(data)) {
            return it->second.c_str();
         }
         EM_LOG_WARNING("Locale: `"  + s + "` has no translation in " + name );
         return s;
      }
      unsigned GetSize() { return data.size(); }

   protected:
      unordered_map<string, string> data;
      string name;
      string original_name;
   };

   // Special case -- English
   class EngDictionary : public Dictionary {
   public:
      EngDictionary() { name ="English"; original_name="English"; }
      static EngDictionary* create() {
         return new EngDictionary();
      }
      virtual const string& Lookup(const string& s) override {
         return s;
      }
      virtual const char* Lookup(const char* s) override {
         return s;
      }
   };



   //////////////////////////////////////////////////////////////////////////


   LocaleManager::LocaleManager()
   {
      EM_LOG_INFO( " ===== Locale manager created ====== " );
      current = nullptr;

      // Loading XML's
      for (auto& f : implemented_langs ) {
         auto fab = Dictionary::create(f);
         if (!fab) {
            EM_LOG_ERROR("Failed load lang XML :" + f + "skipping");
            continue;
         }
         languages.emplace(fab->GetName(), fab);
         EM_LOG_INFO("Loaded: " + f + " with " + fab->GetSize() + " strings" );
      }
      // Put English
      auto en = EngDictionary::create();
      languages.emplace(en->GetName(),en);


      // TODO: Take settings in account

      auto it = languages.find(GetCurrentLanguage());
      if (it == languages.end()) {
         EM_LOG_INFO("Selecting English language as default");
         current = en;
      } else {
         EM_LOG_INFO("Selecting " + it->first + " language");
         current = it->second;
      }

      locale = current->GetName();

      EM_LOG_INFO( " ===== Locale loaded =============== " );
   }

   LocaleManager::~LocaleManager()
   {
      for (auto p : languages) {
         delete p.second;
      }
   }

   const string& LocaleManager::GetLocalisedString( const string& key )
   {
      assert (current);
      return current->Lookup(key);
   }

   const char* LocaleManager::GetLocalisedChar( const char* key )
   {
      assert(current);
      assert(key);
      return current->Lookup(key);
   }

   void LocaleManager::SetLocale( string locale )
   {
      auto it = languages.find(locale);
      if (it == languages.end()) {
         EM_LOG_ERROR("Locale: " + locale + " not loaded ");
         return;
      }
      current = it->second;
   }

   string LocaleManager::GetLocale() const
   {
      assert(current);
      return current->GetName();
   }

   const char* LocaleManager::GetCurrentLanguage()
   {
      auto lang = Application::getInstance()->getCurrentLanguage();
      switch (lang)
      {
      case cocos2d::LanguageType::ENGLISH:
         return "English";
      case cocos2d::LanguageType::CHINESE:
         return "Chinese";
      case cocos2d::LanguageType::FRENCH:
         return "French";
      case cocos2d::LanguageType::ITALIAN:
         return "Italian";
      case cocos2d::LanguageType::GERMAN:
         return "German";
      case cocos2d::LanguageType::SPANISH:
         return "Spanish";
      case cocos2d::LanguageType::RUSSIAN:
         return "Russian";
      case cocos2d::LanguageType::KOREAN:
         return "Korean";
      case cocos2d::LanguageType::JAPANESE:
         return "Japanese";
      case cocos2d::LanguageType::HUNGARIAN:
         return "Hungarian";
      case cocos2d::LanguageType::PORTUGUESE:
         return "Portuguese";
      case cocos2d::LanguageType::ARABIC:
         return "Arabic";
      case cocos2d::LanguageType::NORWEGIAN:
         return "Norwegian";
      case cocos2d::LanguageType::POLISH:
         return "Polish";
      default:
         return "Unsupported";
      }
   }

}

